#ifndef VIPEROS_ATHERIS_X64_DRIVER_FRAMEBUFFER_INSTALL_H
#define VIPEROS_ATHERIS_X64_DRIVER_FRAMEBUFFER_INSTALL_H 1

namespace x64
{
    constexpr int FramebufferInstallError = 1;

    /*
     * Installs the framebuffer with limine's
     * framebuffer request
     *
     * Returns 0 on success
     * Returns 1 if a framebuffer is not found
     */
    int InstallFramebuffer();
}

#endif // VIPEROS_ATHERIS_X64_DRIVER_FRAMEBUFFER_INSTALL_H