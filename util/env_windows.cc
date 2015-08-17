// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/env.h"


namespace leveldb {

class WindowsEnv : public Env {
	public:
	WindowsEnv() {}
	virtual ~WindowsEnv() {}
	
	virtual Status NewSequentialFile(const std::string& fname,
                                   SequentialFile** result) {}

virtual Status NewRandomAccessFile(const std::string& fname,
                                     RandomAccessFile** result) {}
									 
virtual Status NewWritableFile(const std::string& fname,
                                 WritableFile** result) {}
								 
virtual bool FileExists(const std::string& fname){}

virtual Status GetChildren(const std::string& dir,
                             std::vector<std::string>* result){}
							 
virtual Status DeleteFile(const std::string& fname) {}

virtual Status CreateDir(const std::string& dirname) {}

virtual Status DeleteDir(const std::string& dirname){}

virtual Status GetFileSize(const std::string& fname, uint64_t* file_size){}

virtual Status RenameFile(const std::string& src,
                            const std::string& target){}
							
virtual Status LockFile(const std::string& fname, FileLock** lock){}

virtual Status UnlockFile(FileLock* lock){}
virtual void Schedule(
      void (*function)(void* arg),
      void* arg){}
	  
virtual void StartThread(void (*function)(void* arg), void* arg){}

virtual Status GetTestDirectory(std::string* path){}

virtual Status NewLogger(const std::string& fname, Logger** result){}

virtual uint64_t NowMicros(){}

virtual void SleepForMicroseconds(int micros){}

};



Env* Env::Default() {
  return new WindowsEnv();
}

}  // namespace leveldb
