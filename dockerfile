FROM ubuntu:latest

# Expose ports
EXPOSE 8081/tcp
EXPOSE 8081/udp

# Set workdir to folder /app
WORKDIR /app

# Update package updater
RUN apt-get update -y

# Install needed packages
RUN apt-get install sudo -y
RUN sudo apt-get install -y libtbb-dev xorg-dev libglu1-mesa-dev libxrandr-dev curl tar build-essential wget

# Add github.com to known hosts
RUN wget https://github.com/MasterLaplace/Flakkari/releases/latest/download/flakkari-linux.deb

RUN dpkg -i flakkari-linux.deb
RUN mkdir Games

# Launch the generated executable
CMD ["sh", "-c", "flakkari", "Games/", "localhost", "8081"]
