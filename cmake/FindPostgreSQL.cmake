find_path(POSTGRESQL_INCLUDE_DIR libpq-fe.h
   /usr/include/pgsql/
   /usr/local/include/pgsql/
   /usr/include/postgresql/
)

find_library(POSTGRESQL_LIBRARIES NAMES pq)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PostgreSQL DEFAULT_MSG
                                  POSTGRESQL_INCLUDE_DIR POSTGRESQL_LIBRARIES )

mark_as_advanced(POSTGRESQL_INCLUDE_DIR POSTGRESQL_LIBRARIES)