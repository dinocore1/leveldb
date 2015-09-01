// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/env.h"

#include "windows.h"

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
  std::string& mFilename;
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
      result = Slice(scratch, r);
      return Status::OK();
    } else {
      return IOError(mFilename, GetLastError());
    }
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
    return Status::NotSupported("not implemented");
  }
								 
  virtual bool FileExists(const std::string& fname){
    return Status::NotSupported("not implemented");
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
    return Status::NotSupported("not implemented");
  }
	  
  virtual void StartThread(void (*function)(void* arg), void* arg){
    return Status::NotSupported("not implemented");
  }

  virtual Status GetTestDirectory(std::string* path){
    return Status::NotSupported("not implemented");
  }

  virtual Status NewLogger(const std::string& fname, Logger** result){
    return Status::NotSupported("not implemented");
  }

  virtual uint64_t NowMicros(){
    return Status::NotSupported("not implemented");
  }

  virtual void SleepForMicroseconds(int micros){
    return Status::NotSupported("not implemented");
  }

};



Env* Env::Default() {
  return new WindowsEnv();
}

}  // namespace leveldb
