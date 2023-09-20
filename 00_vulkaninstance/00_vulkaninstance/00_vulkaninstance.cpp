// 00_vulkaninstance.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
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
            &debug_messenger_callback, static_cast<void*>(this));


    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_messenger_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {
        std::ostringstream message;

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
                }
                else {
                    message << pCallbackData->pObjects[object].objectHandle;
                }
                if (NULL != pCallbackData->pObjects[object].pObjectName && strlen(pCallbackData->pObjects[object].pObjectName) > 0) {
                    message << ", Name \"" << pCallbackData->pObjects[object].pObjectName << "\"\n";
                }
                else {
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

        MyVulkan& mv = *static_cast<MyVulkan*>(pUserData);
        mv.debug_msg(message);
        return false;  // Don't bail out, but keep going.
    }

    void debug_msg(std::ostringstream const& message) {
        std::cout << message.str() << std::endl;
    }
};


int main() {
    std::cout << "hello world!\n";
    return validation_error;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
