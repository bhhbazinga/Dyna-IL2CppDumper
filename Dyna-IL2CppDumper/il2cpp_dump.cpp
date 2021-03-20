#include "il2cpp_dump.h"

#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <cinttypes>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include "define.h"
#include "log.h"
#include "il2cpp-tabledefs.h"
#include IL2CPPCLASS

#define DO_API(r, n, p) r (*n) p

#include IL2CPPAPI

#undef DO_API

static uintptr_t base_address;

static void init_il2cpp_api(HMODULE module) {
#define DO_API(r, n, p) n = (r (*) p)GetProcAddress(module, #n)

#include IL2CPPAPI

#undef DO_API
}

std::string get_method_modifier(uint16_t flags) {
    std::stringstream output;
    switch (flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) {
        case METHOD_ATTRIBUTE_PRIVATE:
            output << "private ";
            break;
        case METHOD_ATTRIBUTE_PUBLIC:
            output << "public ";
            break;
        case METHOD_ATTRIBUTE_FAMILY:
            output << "protected ";
            break;
        case METHOD_ATTRIBUTE_ASSEM:
        case METHOD_ATTRIBUTE_FAM_AND_ASSEM:
            output << "internal ";
            break;
        case METHOD_ATTRIBUTE_FAM_OR_ASSEM:
            output << "protected internal ";
            break;
    }
    if (flags & METHOD_ATTRIBUTE_STATIC) {
        output << "static ";
    }
    if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
        output << "abstract ";
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            output << "override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_FINAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            output << "sealed override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_NEW_SLOT) {
            output << "virtual ";
        } else {
            output << "override ";
        }
    }
    if (flags & METHOD_ATTRIBUTE_PINVOKE_IMPL) {
        output << "extern ";
    }
    return output.str();
}

std::string dump_method(Il2CppClass* klass) {
    std::stringstream output;
    if (klass->method_count > 0) {
        output << "\n\t// Methods\n";
        void* iter = nullptr;
        const MethodInfo* method = nullptr;
        while ((method = il2cpp_class_get_methods(klass, &iter)) != nullptr) {
            //TODO attribute
            if (method->methodPointer) {
                output << "\t// RVA: 0x";
                output << std::hex << (uint64_t)method->methodPointer - base_address;
                output << " VA: 0x";
                output << std::hex << (uint64_t)method->methodPointer;
            } else {
                output << "\t// RVA: 0x VA: 0x0";
            }
            if (method->slot != 65535) {
                output << " Slot: " << std::dec << method->slot;
            }
            output << "\n\t";
            output << get_method_modifier(method->flags);
            //TODO genericContainerIndex
            const Il2CppType* return_type = method->return_type;
            if (return_type->byref) {
                output << "ref ";
            }
            Il2CppClass* return_class = il2cpp_class_from_type(return_type);
            output << return_class->name << " " << method->name << "(";
            for (int i = 0; i < method->parameters_count; ++i) {
                const ParameterInfo& parameters = method->parameters[i];
                const Il2CppType* parameter_type = parameters.parameter_type;
                unsigned int attrs = parameter_type->attrs;
                if (parameter_type->byref) {
                    if (attrs & PARAM_ATTRIBUTE_OUT && !(attrs & PARAM_ATTRIBUTE_IN)) {
                        output << "out ";
                    } else if (attrs & PARAM_ATTRIBUTE_IN && !(attrs & PARAM_ATTRIBUTE_OUT)) {
                        output << "in ";
                    } else {
                        output << "ref ";
                    }
                } else {
                    if (attrs & PARAM_ATTRIBUTE_IN) {
                        output << "[In] ";
                    }
                    if (attrs & PARAM_ATTRIBUTE_OUT) {
                        output << "[Out] ";
                    }
                }
                Il2CppClass* parameter_class = il2cpp_class_from_type(parameter_type);
                output << parameter_class->name << " " << parameters.name;
                output << ", ";
            }
            if (method->parameters_count > 0) {
                output.seekp(-2, output.cur);
            }
            output << ") { }\n";
            //TODO GenericInstMethod
        }
    }
    return output.str();
}

std::string dump_property(Il2CppClass* klass) {
    std::stringstream output;
    if (klass->property_count > 0) {
        output << "\n\t// Properties\n";
        void* iter = nullptr;
        const PropertyInfo* prop;
        while ((prop = il2cpp_class_get_properties(klass, &iter)) != nullptr) {
            //TODO attribute
            output << "\t";
            Il2CppClass* prop_class = nullptr;
            if (prop->get) {
                output << get_method_modifier(prop->get->flags);
                prop_class = il2cpp_class_from_type(prop->get->return_type);
            } else if (prop->set) {
                output << get_method_modifier(prop->set->flags);
                prop_class = il2cpp_class_from_type(prop->set->parameters[0].parameter_type);
            } else {
                LOG_ERR("Invalid prop, prop->name=%s", prop->name);
            }
            output << prop_class->name << " " << prop->name << " { ";
            if (prop->get) {
                output << "get; ";
            }
            if (prop->set) {
                output << "set; ";
            }
            output << "}\n";
        }
    }
    return output.str();
}

std::string dump_field(Il2CppClass* klass) {
    std::stringstream output;
    if (klass->field_count > 0) {
        output << "\n\t// Fields\n";
        void* iter = nullptr;
        const FieldInfo* field = nullptr;
        while ((field = il2cpp_class_get_fields(klass, &iter)) != nullptr) {
            //TODO attribute
            output << "\t";
            unsigned int attrs = field->type->attrs;
            switch (attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK) {
                case FIELD_ATTRIBUTE_PRIVATE:
                    output << "private ";
                    break;
                case FIELD_ATTRIBUTE_PUBLIC:
                    output << "public ";
                    break;
                case FIELD_ATTRIBUTE_FAMILY:
                    output << "protected ";
                    break;
                case FIELD_ATTRIBUTE_ASSEMBLY:
                case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
                    output << "internal ";
                    break;
                case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
                    output << "protected internal ";
                    break;
            }
            if (attrs & FIELD_ATTRIBUTE_LITERAL) {
                output << "const ";
            } else {
                if (attrs & FIELD_ATTRIBUTE_STATIC) {
                    output << "static ";
                }
                if (attrs & FIELD_ATTRIBUTE_INIT_ONLY) {
                    output << "readonly ";
                }
            }
            Il2CppClass* field_class = il2cpp_class_from_type(field->type);
            output << field_class->name << " " << field->name;
            output << "; // 0x" << std::hex << field->offset << "\n";
        }
    }
    return output.str();
}

std::string dump_type(const Il2CppType* type) {
    std::stringstream output;
    Il2CppClass* klass = il2cpp_class_from_type(type);
    output << "\n// Namespace: " << klass->namespaze << "\n";
    int flags = klass->flags;
    if (flags & TYPE_ATTRIBUTE_SERIALIZABLE) {
        output << "[Serializable]\n";
    }
    //TODO attribute
    switch (flags & TYPE_ATTRIBUTE_VISIBILITY_MASK) {
        case TYPE_ATTRIBUTE_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_PUBLIC:
            output << "public ";
            break;
        case TYPE_ATTRIBUTE_NOT_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM:
        case TYPE_ATTRIBUTE_NESTED_ASSEMBLY:
            output << "internal ";
            break;
        case TYPE_ATTRIBUTE_NESTED_PRIVATE:
            output << "private ";
            break;
        case TYPE_ATTRIBUTE_NESTED_FAMILY:
            output << "protected ";
            break;
        case TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM:
            output << "protected internal ";
            break;
    }
    if (flags & TYPE_ATTRIBUTE_ABSTRACT && flags & TYPE_ATTRIBUTE_SEALED) {
        output << "static ";
    } else if (!(flags & TYPE_ATTRIBUTE_INTERFACE) && flags & TYPE_ATTRIBUTE_ABSTRACT) {
        output << "abstract ";
    } else if (!klass->valuetype && !klass->enumtype && flags & TYPE_ATTRIBUTE_SEALED) {
        output << "sealed ";
    }
    if (flags & TYPE_ATTRIBUTE_INTERFACE) {
        output << "interface ";
    } else if (klass->enumtype) {
        output << "enum ";
    } else if (klass->valuetype) {
        output << "struct ";
    } else {
        output << "class ";
    }
    output << klass->name; //TODO genericContainerIndex
    std::vector<std::string> extends;
    if (!klass->valuetype && !klass->enumtype && klass->parent) {
        const Il2CppType* parent_type = il2cpp_class_get_type(klass->parent);
        if (parent_type->type != IL2CPP_TYPE_OBJECT) {
            extends.emplace_back(klass->parent->name);
        }
    }
    if (klass->interfaces_count > 0) {
        void* iter = nullptr;
        Il2CppClass* itf;
        while ((itf = il2cpp_class_get_interfaces(klass, &iter)) != nullptr) {
            extends.emplace_back(itf->name);
        }
    }
    if (!extends.empty()) {
        output << " : " << extends[0];
        for (size_t i = 1; i < extends.size(); ++i) {
            output << ", " << extends[i];
        }
    }
    output << " // TypeDefIndex: " << type->data.klassIndex << "\n{";
    output << dump_field(klass);
    output << dump_property(klass);
    output << dump_method(klass);
    //TODO EventInfo
    output << "}\n";
    return output.str();
}

void il2cpp_dump(HMODULE module, const char* outDir) {
    LOG_INFO("UnityVersion: %s", STRINGIFY_MACRO(UnityVersion));
#ifdef VersionAboveV24
    LOG_INFO("VersionAboveV24: on");
#else
    LOG_INFO("VersionAboveV24: off");
#endif
    LOG_INFO("il2cpp_handle: %p", module);
    base_address = reinterpret_cast<uintptr_t>(module);
    init_il2cpp_api(module);
    Il2CppDomain* domain = il2cpp_domain_get();
    il2cpp_thread_attach(domain);
    size_t size;
    const Il2CppAssembly** assemblies = il2cpp_domain_get_assemblies(domain, &size);
    std::stringstream image_output;
    std::map<TypeDefinitionIndex, std::string> outPuts;
    LOG_INFO("il2cpp_base: %" PRIx64"", base_address);
#ifdef VersionAboveV24
    //使用il2cpp_image_get_class
    for (size_t i = 0; i < size; ++i) {
        const Il2CppImage* image = il2cpp_assembly_get_image(assemblies[i]);
#ifdef WHITE_LIST
        if (kDumpAssemblies.find(image->name) == kDumpAssemblies.end()) {
            continue;
        }
#endif
        image_output << "// Image " << i << ": " << image->name << " - " << image->typeStart << "\n";
        size_t classCount = il2cpp_image_get_class_count(image);
        for (size_t j = 0; j < classCount; ++j) {
            const Il2CppClass* klass = il2cpp_image_get_class(image, j);
            const Il2CppType* type = il2cpp_class_get_type(const_cast<Il2CppClass*>(klass));
            TypeDefinitionIndex klassIndex = type->data.klassIndex;
            outPuts.try_emplace(klassIndex, dump_type(type));
        }
    }
#else
    //使用反射
    const Il2CppImage* corlib = il2cpp_get_corlib();
    Il2CppClass* assemblyClass = il2cpp_class_from_name(corlib, "System.Reflection", "Assembly");
    const MethodInfo* assemblyLoad = il2cpp_class_get_method_from_name(assemblyClass, "Load", 1);
    const MethodInfo* assemblyGetTypes = il2cpp_class_get_method_from_name(assemblyClass, "GetTypes", 0);
    if (assemblyLoad && assemblyLoad->methodPointer) {
        LOG_INFO("Assembly::Load: %p", assemblyLoad->methodPointer);
    } else {
        LOG_INFO("miss Assembly::Load");
        return;
    }
    if (assemblyGetTypes && assemblyGetTypes->methodPointer) {
        LOG_INFO("Assembly::GetTypes: %p", assemblyGetTypes->methodPointer);
    } else {
        LOG_INFO("miss Assembly::GetTypes");
        return;
    }
#ifdef VersionAboveV24
    typedef void* (*Assembly_Load_ftn)(Il2CppString*, void*);
#else
    typedef void* (*Assembly_Load_ftn)(void*, Il2CppString*, void*);
#endif
    typedef Il2CppArray* (*Assembly_GetTypes_ftn)(void*, void*);
    LOG_INFO("dumping...");
    for (int i = 0; i < size; ++i) {
        const Il2CppImage* image = il2cpp_assembly_get_image(assemblies[i]);
#ifdef WHITE_LIST
        if (kDumpAssemblies.find(image->name) == kDumpAssemblies.end()) {
            continue;
        }
#endif
        std::string imageName = std::string(image->name);
        size_t pos = imageName.rfind('.');
        std::string imageNameNoExt = imageName.substr(0, pos);
        Il2CppString* assemblyFileName = il2cpp_string_new(imageNameNoExt.c_str());
#ifdef VersionAboveV24
        auto reflectionAssembly = ((Assembly_Load_ftn)assemblyLoad->methodPointer)(
            assemblyFileName, nullptr);
#else
        auto reflectionAssembly = ((Assembly_Load_ftn)assemblyLoad->methodPointer)(nullptr,
                                                                                   assemblyFileName,
                                                                                   nullptr);
#endif
        auto reflectionTypes = ((Assembly_GetTypes_ftn)assemblyGetTypes->methodPointer)(
            reflectionAssembly, nullptr);
        auto items = reflectionTypes->vector;
        for (int j = 0; j < reflectionTypes->max_length; ++j) {
            Il2CppClass* klass = il2cpp_class_from_system_type((Il2CppReflectionType*)items[j]);
            const Il2CppType* type = il2cpp_class_get_type(klass);
            //LOGD("type name : %s", il2cpp_type_get_name(type));
            TypeDefinitionIndex klassIndex = type->data.klassIndex;
            outPuts.try_emplace(klassIndex, dump_type(type));
        }
    }
#endif
    LOG_INFO("write dump file");
    std::string outPath = std::string(outDir).append("\\dump.cs");
    std::ofstream outStream(outPath);
    outStream << image_output.str();
    for (auto it = outPuts.begin(); it != outPuts.end(); it++) {
        outStream << it->second;
    }
    outStream.close();
    LOG_INFO("dump done!");
}