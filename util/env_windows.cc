#include "windows.h"

#include "leveldb/env.h"

namespace leveldb {
  
static Status IOError(const std::string& context, DWORD errorCode) {
  LPTSTR errorText = NULL;

  FormatMessage(
   FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,  
   NULL,
   errorCode,
   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
   (LPTSTR)&errorText, 
   0,
   NULL); 

   std::string errorMsg;
   if ( NULL != errorText ) {
     errorMsg = errorText;
     LocalFree(errorText);
     errorText = NULL;
   }
   
   return Status::IOError(context, errorMsg);
}
  
class WindowsSequentialFile : public SequentialFile {
  private:
  const std::string& mFilename;
  HANDLE hFile;
  
  public:
  WindowsSequentialFile(const std::string& filename, const HANDLE file)
  : mFilename(filename), hFile(file) { }
  
  virtual ~WindowsSequentialFile() {
    CloseHandle(hFile);
  }
  
  virtual Status Read(size_t n, Slice* result, char* scratch) {
    DWORD r;
    if(ReadFile(hFile, scratch, n, &r, NULL)){
      *result = Slice(scratch, r);
      return Status::OK();
    } else {
      return IOError(mFilename, GetLastError());
    }
  }
  
  virtual Status Skip(uint64_t n) {
    LARGE_INTEGER li;
    li.QuadPart = n;
    
    li.LowPart = SetFilePointer(hFile, li.LowPart, &li.HighPart, FILE_CURRENT);
    if(li.LowPart == INVALID_SET_FILE_POINTER) {
      return IOError(mFilename, GetLastError());
    } else {
      return Status::OK();
    }
  }
};

class WindowsWriteableFile : public WritableFile {
  private:
  const std::string& mFilename;
  HANDLE hFile;
  
  
  public:
  WindowsWriteableFile(const std::string& filename, const HANDLE file)
  : mFilename(filename), hFile(file) { }
  
  ~WindowsWriteableFile() {
    CloseHandle(hFile);
  }
  
  virtual Status Append(const Slice& data) {
    
  }
  virtual Status Close() {
    
  }
  virtual Status Flush() {
    
  }
  virtual Status Sync() {
    
  }
};

class WindowsEnv : public Env {
	public:
  WindowsEnv() {}
  virtual ~WindowsEnv() {}
	
  virtual Status NewSequentialFile(const std::string& fname,
                                   SequentialFile** result) {
    HANDLE hFile = CreateFile(fname.c_str(), 
       GENERIC_READ,
       FILE_SHARE_READ, 
       NULL, 
       OPEN_EXISTING, 
       FILE_ATTRIBUTE_NORMAL, 
       NULL);
       
       if (hFile == INVALID_HANDLE_VALUE) {
         *result = NULL;
         return IOError(fname, GetLastError());
       } else {
         *result = new WindowsSequentialFile(fname, hFile);
         return Status::OK();
       }              
  }

  virtual Status NewRandomAccessFile(const std::string& fname,
                                     RandomAccessFile** result) {
    return Status::NotSupported("not implemented");
  }
									 
  virtual Status NewWritableFile(const std::string& fname,
                                 WritableFile** result) {
    
    
    HANDLE hFile = CreateFile(fname.c_str(),
      GENERIC_WRITE,
      FILE_SHARE_WRITE,
      NULL,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      NULL);
      
      if(hFile == INVALID_HANDLE_VALUE) {
        *result = NULL;
        return IOError(fname, GetLastError());
      } else {
        *result = new WindowsWriteableFile(fname, hFile);
        return Status::OK();
      }
    
  }
								 
  virtual bool FileExists(const std::string& fname){
    //TODO implement
    return false;
  }

  virtual Status GetChildren(const std::string& dir,
                             std::vector<std::string>* result){
    return Status::NotSupported("not implemented");
  
  }
  
  virtual Status DeleteFile(const std::string& fname) {
    return Status::NotSupported("not implemented");
  }

  virtual Status CreateDir(const std::string& dirname) {
    return Status::NotSupported("not implemented");
  }

  virtual Status DeleteDir(const std::string& dirname){
    return Status::NotSupported("not implemented");
  }

  virtual Status GetFileSize(const std::string& fname, uint64_t* file_size){
    return Status::NotSupported("not implemented");
  }

  virtual Status RenameFile(const std::string& src,
                            const std::string& target){
    return Status::NotSupported("not implemented");
  }
							
  virtual Status LockFile(const std::string& fname, FileLock** lock){
    return Status::NotSupported("not implemented");
  }

  virtual Status UnlockFile(FileLock* lock){
    return Status::NotSupported("not implemented");
  }
  
  virtual void Schedule(
      void (*function)(void* arg),
      void* arg){
    
    //TODO implement
  }
	  
  virtual void StartThread(void (*function)(void* arg), void* arg){
    //TODO implement
  }

  virtual Status GetTestDirectory(std::string* path){
    return Status::NotSupported("not implemented");
  }

  virtual Status NewLogger(const std::string& fname, Logger** result){
    return Status::NotSupported("not implemented");
  }

  virtual uint64_t NowMicros(){
    //TODO implement
    return 0;
  }

  virtual void SleepForMicroseconds(int micros){
  }

};



Env* Env::Default() {
  return new WindowsEnv();
}

}  // namespace leveldb
