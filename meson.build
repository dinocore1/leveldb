project('leveldb', 'cpp')

if get_option('shared_lib')
  libtype = 'shared_library'
else
  libtype = 'static_library'
endif

db_src = [ 'db/builder.cc', 'db/c.cc', 'db/db_impl.cc', 'db/db_iter.cc', 'db/dbformat.cc',
'db/dumpfile.cc', 'db/filename.cc', 'db/log_reader.cc', 'db/log_writer.cc',
'db/memtable.cc', 'db/repair.cc', 'db/table_cache.cc', 'db/version_edit.cc',
'db/version_set.cc', 'db/write_batch.cc',
]

util_src = [ 'util/arena.cc', 'util/bloom.cc', 'util/cache.cc', 'util/coding.cc',
'util/comparator.cc', 'util/crc32c.cc', 'util/env.cc',
'util/filter_policy.cc', 'util/hash.cc', 'util/histogram.cc', 'util/logging.cc',
'util/options.cc', 'util/status.cc' ]

table_src = ['table/block.cc', 'table/block_builder.cc', 'table/filter_block.cc',
'table/format.cc', 'table/iterator.cc', 'table/merger.cc', 'table/table.cc',
'table/table_builder.cc', 'table/two_level_iterator.cc' ]

cpp_args = []
port_src = []

if meson.is_cross_build()
  if host_machine.system() == 'android'
    port_src = [ 'port/port_posix.cc', 'util/env_posix.cc' ]
    cpp_args = [ '-DOS_ANDROID' , '-DLEVELDB_PLATFORM_POSIX', '-D_REENTRANT', '-fno-builtin-memcmp' ]
  elif host_machine.system() == 'mingw'
    port_src = [ 'port/port_win.cc', 'util/env_windows.cc' ]
    cpp_args = [ '-D_WIN32_WINNT=0x0600', '-DLEVELDB_PLATFORM_WINDOWS' ]
  endif
else
  if host_machine.system() == 'linux'
    port_src = [ 'port/port_posix.cc', 'util/env_posix.cc' ]
    cpp_args = [ '-DOS_LINUX', '-DLEVELDB_PLATFORM_POSIX' ]
  elif host_machine.system() == 'darwin'
    port_src = [ 'port/port_posix.cc', 'util/env_posix.cc' ]
    cpp_args = [ '-DOS_MACOSX', '-DLEVELDB_PLATFORM_POSIX' ]
  elif host_machine.system() == 'windows'
    port_src = [ 'port/port_win.cc', 'util/env_windows.cc' ]
    cpp_args = [ '-DLEVELDB_PLATFORM_WINDOWS' ]
  endif
endif

incdir = include_directories('include')

leveldb_lib = build_target('leveldb', [db_src, util_src, table_src, port_src],
  include_directories : incdir,
  target_type : libtype,
  cpp_args : cpp_args
)

dep = declare_dependency(link_with : leveldb_lib, include_directories : incdir)
