.PHONY: build
build:
	docker build -t soprateam17/ki-client .

.PHONY: run
run:
	docker run --rm --net=host soprateam17/ki-client