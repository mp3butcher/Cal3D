import os

import ctypes
import tempfile

if __debug__:
    cal3d_dll  = ctypes.cdll.cal3d_d
else:
    cal3d_dll = ctypes.cdll.cal3d


class Cal3d:
    def __init__(self):
        self.loader_ = cal3d_dll.CalLoader_New()
        assert self.loader_
        self.saver_ = cal3d_dll.CalSaver_New()
        assert self.saver_
        self.typeToBinaryExtensionMap_ = { "CoreAnimation" : "caf",
                                           "CoreMaterial"  : "crf",
                                           "CoreAnimatedMorph"  : "cpf",
                                           "CoreMesh"      : "cmf",
                                           "CoreSkeleton"  : "csf" }
        self.typeToXmlExtensionMap_ = { "CoreAnimation" : "xaf",
                                        "CoreMaterial"  : "xrf",
                                        "CoreAnimatedMorph"  : "xpf",
                                        "CoreMesh"      : "xmf",
                                        "CoreSkeleton"  : "xsf" }
        self.xmlExtensionToTypeMap_ = {}
        for key, value in self.typeToXmlExtensionMap_.items() + self.typeToBinaryExtensionMap_.items():
            self.xmlExtensionToTypeMap_[".%s" % value] = key

    def cal3dTypeForPath(self, path):
        name, ext = os.path.splitext(path)
        try:
            return self.xmlExtensionToTypeMap_[ext]
        except KeyError:
            return None
        
    def convertToBinary(self, calCoreType, data):
        extension = self.typeToBinaryExtensionMap_[calCoreType]
        return self.__convert(calCoreType=calCoreType, data=data, extension=extension)

    def convertToXml(self, calCoreType, data):
        extension = self.typeToXmlExtensionMap_[calCoreType]
        return self.__convert(calCoreType=calCoreType, data=data, extension=extension)

    def __convert(self, calCoreType, data, extension):
        loaderFunc = getattr(cal3d_dll, "CalLoader_Load"+calCoreType+"FromBuffer")
        saverFunc = getattr(cal3d_dll, "CalSaver_Save"+calCoreType)
        if not loaderFunc:
            raise "could not find Loader for calCoreType %s" % calCoreType
        if not saverFunc:
            raise "could not find Saver for calCoreType %s" % calCoreType
        object = loaderFunc(self.loader_, data, len(data))
        if not object:
            cal3d_dll.CalError_PrintLastError()
            raise "could not load data (len %s) for calCoreType %s" % (len(data), calCoreType)
        path = "%s.%s" % (tempfile.mktemp(), extension)
        try:
            r = saverFunc(self.saver_, path, object)
            if not r:
                cal3d_dll.CalError_PrintLastError();
                raise "could not save object 0x%x (calCoreType=%s) to path %s" % (object, calCoreType, path)
            f = open(path, "rb")
            ret = f.read()
            f.close()
        finally:
            if os.path.exists(path):
                os.remove(path)

        return ret

    def __del__(self):
        cal3d_dll.CalLoader_Delete(self.loader_)
        cal3d_dll.CalSaver_Delete(self.saver_)
        
        
