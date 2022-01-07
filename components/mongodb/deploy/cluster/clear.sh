#!/bin/bash
sudo docker-compose down

sudo rm -rf mdb-config-1/* 
sudo rm -rf mdb-config-2/* 
sudo rm -rf mdb-config-3/* 
sudo rm -rf mdb-r1-1/*
sudo rm -rf mdb-r1-2/*
sudo rm -rf mdb-r1-3/*
sudo rm -rf mdb-r2-1/*
sudo rm -rf mdb-r2-2/*
sudo rm -rf mdb-r2-3/*
sudo rm -rf mdb-r3-1/*
sudo rm -rf mdb-r3-2/*
sudo rm -rf mdb-r3-3/*

sudo docker-compose up -d
