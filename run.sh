#!/bin/bash
docker build -t clink .
docker run -p 8080:8080 clink