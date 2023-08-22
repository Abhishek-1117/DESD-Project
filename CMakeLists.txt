idf_component_register (SRCS "aht20_slave.c"
                        INCLUDE_DIRS "."
			REQUIRES "driver" i2c_master)