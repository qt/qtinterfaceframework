# How to build

	qmake
	make
	make install
	

# Configuration

By running qmake for the first time, a configuration step runs and tries to detect a configuration that works on your
system. At the end a configuration summary is printed, incl. some notes on what the configuration might be missing
due to dependencies missing.

Please read this configuration output first and try to fix the dependencies before creating bug reports.

The configuration step is based on Qt's configure system and also offers some command-line arguments for enabling/disabling
certain features. More about how to configure interfaceframework can be found here:

https://doc-snapshots.qt.io/interfaceframework/configuration.html
