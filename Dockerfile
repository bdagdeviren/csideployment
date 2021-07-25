FROM conan AS build
WORKDIR /app
COPY . .
RUN cd go/ip && go test && go build -buildmode=c-shared -ldflags="-w -s" -gcflags=all=-l -gcflags=all=-B -o ../../libs/ip/libip.so main.go
RUN mkdir conan && cd conan && conan install .. --build=missing
RUN mkdir build && cd build && cmake .. && cmake --build . && cd bin && staticx csideployment csideployment-static && rm -rf /tmp/*

#gcr.io/distroless/static
FROM scratch
WORKDIR /app
COPY --from=build /app/build/bin/csideployment-static /app/csideployment-static
COPY --from=build /tmp /tmp
ENTRYPOINT ["/app/csideployment-static"]