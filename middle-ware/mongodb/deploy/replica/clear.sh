#!/bin/bash
sudo docker-compose down
sudo rm -rf mdb-1/* 
sudo rm -rf mdb-2/* 
sudo rm -rf mdb-3/* 
sudo docker-compose up -d
