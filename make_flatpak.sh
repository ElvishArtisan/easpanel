#!/bin/sh

# make_flatpak.sh

flatpak-builder --verbose --force-clean --user --install-deps-from=flathub --repo=repo flatpak com.paravelsystems.easpanel.yml
flatpak build-bundle repo easpanel.flatpak com.paravelsystems.easpanel
