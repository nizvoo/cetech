# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|

    config.vm.define "linux64" do |linux64|
        linux64.vm.box = "linux64"
        linux64.vm.box = "bugyt/archlinux"

        linux64.vm.provision "shell", inline: <<-SHELL
            sudo pacman --noconfirm -S mono
        SHELL
    end
end
