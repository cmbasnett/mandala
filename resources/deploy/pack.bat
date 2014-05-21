pack.exe

SET resource_directory="../../Debug/resources"

MKDIR %resource_directory%

COPY "armada.pack" %resource_directory% /Y
COPY "armada.app" %resource_directory% /Y