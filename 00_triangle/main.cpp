#include <iostream>
#include "vulkan/vulkan.hpp"

static int validation_error = 0;

class MyVulkan {
public:
    void init_vk() {
        // todo: validation layers
        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                                        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                                        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
        auto debug_utils_create_info = vk::DebugUtilsMessengerCreateInfoEXT({}, severityFlags, messageTypeFlags,
                                                                            &debug_messenger_callback, static_cast<void *>(this));


    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_messenger_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                              VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                              void *pUserData) {
        std::ostringstream message;
        MyVulkan &demo = *static_cast<MyVulkan *>(pUserData);

        message << vk::to_string(vk::DebugUtilsMessageSeverityFlagBitsEXT(messageSeverity));
        message << " : " + vk::to_string(vk::DebugUtilsMessageTypeFlagsEXT(messageType));

        if (vk::DebugUtilsMessageTypeFlagsEXT(messageType) & vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation) {
            validation_error = 1;
        }

        message << " - Message Id Number: " << std::to_string(pCallbackData->messageIdNumber);
        message << " | Message Id Name: " << pCallbackData->pMessageIdName << "\n\t" << pCallbackData->pMessage << "\n";

        if (pCallbackData->objectCount > 0) {
            message << "\n\tObjects - " << pCallbackData->objectCount << "\n";
            for (uint32_t object = 0; object < pCallbackData->objectCount; ++object) {
                message << "\t\tObject[" << object << "] - "
                        << vk::to_string(vk::ObjectType(pCallbackData->pObjects[object].objectType)) << ", Handle ";

                // Print handle correctly if it is a dispatchable handle - aka a pointer
                VkObjectType t = pCallbackData->pObjects[object].objectType;
                if (t == VK_OBJECT_TYPE_INSTANCE || t == VK_OBJECT_TYPE_PHYSICAL_DEVICE || t == VK_OBJECT_TYPE_DEVICE ||
                    t == VK_OBJECT_TYPE_COMMAND_BUFFER || t == VK_OBJECT_TYPE_QUEUE) {
                    message << reinterpret_cast<void*>(static_cast<uintptr_t>(pCallbackData->pObjects[object].objectHandle));
                } else {
                    message << pCallbackData->pObjects[object].objectHandle;
                }
                if (NULL != pCallbackData->pObjects[object].pObjectName && strlen(pCallbackData->pObjects[object].pObjectName) > 0) {
                    message << ", Name \"" << pCallbackData->pObjects[object].pObjectName << "\"\n";
                } else {
                    message << "\n";
                }
            }
        }
        if (pCallbackData->cmdBufLabelCount > 0) {
            message << "\n\tCommand Buffer Labels - " << pCallbackData->cmdBufLabelCount << "\n";
            for (uint32_t cmd_buf_label = 0; cmd_buf_label < pCallbackData->cmdBufLabelCount; ++cmd_buf_label) {
                message << "\t\tLabel[" << cmd_buf_label << "] - " << pCallbackData->pCmdBufLabels[cmd_buf_label].pLabelName << " { "
                        << pCallbackData->pCmdBufLabels[cmd_buf_label].color[0] << ", "
                        << pCallbackData->pCmdBufLabels[cmd_buf_label].color[1] << ", "
                        << pCallbackData->pCmdBufLabels[cmd_buf_label].color[2] << ", "
                        << pCallbackData->pCmdBufLabels[cmd_buf_label].color[2] << "}\n";
            }
        }

        std::cout << message.str() << std::endl;  // use endl to force a flush
        return false;  // Don't bail out, but keep going.
    }
};


int main() {
    std::cout << "hello world!\n";
    return validation_error;
}
