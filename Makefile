
TARGET = can-watch
IMAGE_NAME := rpi-cross-compile

PWD = $(shell pwd)
USER_ID = $(shell id -u)
GROUP_ID = $(shell id -g)
BUILD_DIR = build

CONTAINER_RUN := docker run --rm --privileged -t -v $(PWD):/project --user "$(USER_ID):$(GROUP_ID)" $(IMAGE_NAME) bash -c

all: $(TARGET)

can-watch: docker-build
	$(CONTAINER_RUN) "aarch64-linux-gnu-g++ -Wno-packed-bitfield-compat can_message_dump.cpp -I magic_enum -I emio -o $(TARGET) -std=c++20"

transfer: $(TARGET)
	scp $(TARGET) smbr:~

docker-build: docker-clean
	docker build . --tag $(IMAGE_NAME) --build-arg USER_ID=$(USER_ID) --build-arg GROUP_ID=$(GROUP_ID)

docker-shell:
	docker exec -it -t --privileged $(IMAGE_NAME) /bin/sh -l

docker-deamon:
	docker run -d -it --name $(IMAGE_NAME) -p 3333:3333 --privileged -v /dev:/dev -v $(PWD):/project $(IMAGE_NAME)

docker-stop:
	docker stop $(IMAGE_NAME) 2> /dev/null > /dev/null || true

docker-clean: docker-stop
	docker rm $(IMAGE_NAME) 2> /dev/null > /dev/null || true
