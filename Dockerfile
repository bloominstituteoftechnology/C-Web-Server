FROM gcc:latest
WORKDIR /usr/src/src
COPY . /usr/src
RUN make
CMD ["./server"]