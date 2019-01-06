FROM base/devel

WORKDIR /builds/MTIpython/MTIpython
VOLUME /app

RUN pacman -Syyu --noconfirm
RUN pacman-db-upgrade
RUN pacman -S --noconfirm cmake eigen boost boost-libs gtest