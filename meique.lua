
libPng = findPackage("libpng", REQUIRED)

png2raw = Executable:new("png2raw")
png2raw:addFiles("main.cpp")
png2raw:usePackage(libPng)
