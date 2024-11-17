# ME : <p align="center">Flakkari<br><br><img src="docs/Images/Flakkari.png" alt="Flakkari logo" style="height: 200px"></p>

<p align="center">
    <h3 align="center">📦 Download &#xa0; : &#xa0;
    <a href="https://github.com/MasterLaplace/Flakkari/releases/latest/">
        <img src="https://img.shields.io/github/v/release/MasterLaplace/Flakkari.svg?label=Latest%20Version&style=for-the-badge" alt="Latest Version">
    </a>&#xa0; | &#xa0;
    📜 License &#xa0; : &#xa0;
    <a href="https://github.com/MasterLaplace/Flakkari/blob/main/LICENSE">
        <img src="https://img.shields.io/badge/License-MIT-brightgreen.svg?style=for-the-badge" alt="License: MIT">
    </a>
    </h3>
</p>
<p align="center">
    <h3 align="center">🌐 Supported Platforms
    &#xa0; : &#xa0;
    <img
        src="https://img.shields.io/badge/Linux-blue?style=for-the-badge"
        alt="Linux"
        href="https://github.com/MasterLaplace/Flakkari/releases/latest"
    > &#xa0; | &#xa0;
    <img
        src="https://img.shields.io/badge/Windows-blue?style=for-the-badge"
        alt="Windows"
    > &#xa0; | &#xa0;
    <img
        src="https://img.shields.io/badge/MacOS-blue?style=for-the-badge"
        alt="MacOS"
    >
    </h3>
</p>
<p align="center">
    <h3 align="center">📡 Supported Protocol
        &#xa0; : &#xa0;
        <img src="https://img.shields.io/badge/UDP-blue?style=for-the-badge" alt="UDP">
        &#xa0; | &#xa0;
        <img src="https://img.shields.io/badge/TCP-blue?style=for-the-badge" alt="TCP">
    </h3>
</p>


## :placard: Table of Contents
- [Description](#description)
- [Running](#running)
- [Docker](#docker)
- [Documentation](#documentation)
- [Latest Release](#latest-release)
- [License](#license)
- [Contacts](#contacts)


<div id='description'/>

### :pencil: **DESCRIPTION**

Flakkari is a UDP server created for the R-Type Epitech project.<br>
It is a server that allows you to play games in a network environment.<br>
The server is multi-threaded and can handle multiple games at the same time.<br>
The server is also able to handle multiple clients at the same time.<br>

> [!NOTE]
> The project is still under development and may not be stable.


<div id='running'/>

### :truck: **RUNNING**

```shell
# Clone repository
$> git clone https://github.com/MasterLaplace/Flakkari.git
$> cd Flakkari

# Create a build directory
$> mkdir build
$> cd build

# Configure the project
(build)$> cmake .. && cmake --build .

# Or configure the project with Ninja
(build)$> cmake -G Ninja .. && cmake --build .

# Run the server executable
(build)$> ./flakkari <GamesDir> <ip> <port>
```

other build commands:
```shell
# Install the project
(build)$> sudo cmake --build . --target install

# Build project documentation
(build)$> cmake --build . --target doc

# Build project package
(build)$> cmake --build . --config Release --target package

# Install the package
# For windows systems
(build)$> flakkari-<version>-win64.exe

# For macos systems
(build)$> sudo installer -pkg flakkari-<version>-Darwin.pkg -target /

# For redhat based systems
(build)$> sudo rpm -i flakkari-<version>-Linux.rpm

# For debian based systems
(build)$> sudo dpkg -i flakkari-<version>-Linux.deb
```

<div id='docker'/>

### :whale: **DOCKER RUN**

```bash
# install docker on ubuntu
$ sudo apt-get update
$ sudo apt-get install docker-ce docker-ce-cli containerd.io

# check docker version (optional)
$ docker --version
> Docker version 24.0.7, build afdd53b

# build docker image
$ docker build -t flakkari .

# run docker image
$ docker run Flakkari -p 4242:4242

# list docker images
$ docker ps

# stop docker image
$ docker stop <container-id>

# remove docker image
$ docker rm <container-id>
```


<div id='documentation'/>

### :wrench: **DOCUMENTATION**

Wiki:
- [**Technical Documentation & Wiki**](https://MasterLaplace.github.io/Flakkari/)

For detailed documentation on the Flakkari protocol, see:
- [**Flakkari Protocol Documentation**](docs/RFC.txt)


## :rocket: LATEST RELEASE

> :clipboard: [Change Log](CHANGELOG.md).

- [![Latest Release](https://img.shields.io/github/v/release/MasterLaplace/Flakkari.svg?label=version)](https://github.com/MasterLaplace/Flakkari/releases/latest/)


<div id='license'/>

## :scroll: **LICENSE**

This project is licensed under the terms of the [MIT License](./LICENSE).

Copyright © 2023-2024 [Master_Laplace](https://github.com/MasterLaplace).


<div id='contacts'/>

### :mailbox_with_mail: **CONTACTS**

This server project has been carried out by:

<table align="center">
    <tbody>
        <tr>
            <td align="center"><a href="https://github.com/MasterLaplace/"><img src="https://avatars.githubusercontent.com/MasterLaplace?v=4?s=100" width="100px;" alt="MasterLaplace"/><br/><a href="https://github.com/MasterLaplace/"><img src="https://img.shields.io/github/followers/MasterLaplace?label=Master_Laplace&style=social" alt="Master_Laplace"/></a></td>
        </tr>
    </tbody>
</table>

---
<p align="center">ME.inc</p>
