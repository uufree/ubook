#!/bin/bash
sudo docker-compose down
rm -rf data/cluster-0/* 
rm -rf data/cluster-1/* 
rm -rf data/cluster-2/* 
rm -rf data/cluster-3/* 
rm -rf data/cluster-4/* 
rm -rf data/cluster-5/* 
sudo docker-compose up -d
