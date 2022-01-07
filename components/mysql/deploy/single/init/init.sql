DROP DATABASE IF EXISTS feature_service;
CREATE DATABASE feature_service DEFAULT CHARACTER SET = utf8mb4;
-- 创建普通用户
grant all PRIVILEGES on feature_service.* to sensetime@'%' identified by 'sensetime';
flush privileges;

USE feature_service;

#----------------------------------------------------------------------------------
DROP TABLE IF EXISTS `groups`;
CREATE TABLE `groups`
(
    id           bigint        NOT NULL AUTO_INCREMENT PRIMARY KEY                              COMMENT '自增id',
    name         varchar(64)   NOT NULL                                                         COMMENT '特征id',
    db_id        varchar(64)   NOT NULL                                                         COMMENT '特征id',
    description  varchar(128)  NOT NULL                                                         COMMENT '特征类型',
    cover_base64 mediumtext    NOT NULL                                                         COMMENT '特征类型',
    quality_threshold   float         NOT NULL DEFAULT 0                                               COMMENT '入库质量阈值',
    verify_threshold    float         NOT NULL DEFAULT 0                                               COMMENT '出库相似度阈值',

    created_at   datetime      NOT NULL DEFAULT CURRENT_TIMESTAMP                               COMMENT '创建时间',
    updated_at   datetime      NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP   COMMENT '更新时间',
    KEY (db_id, name)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE = utf8mb4_bin COMMENT 'groups';

#----------------------------------------------------------------------------------
DROP TABLE IF EXISTS `labels`;
CREATE TABLE `labels`
(
    id           bigint         NOT NULL AUTO_INCREMENT PRIMARY KEY                              COMMENT '自增id',
    name         varchar(64)    NOT NULL                                                         COMMENT '标签名',
    db_id        varchar(64)   NOT NULL                                                         COMMENT '特征id',

    created_at   datetime       NOT NULL DEFAULT CURRENT_TIMESTAMP                               COMMENT '创建时间',
    updated_at   datetime       NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP   COMMENT '更新时间',
    UNIQUE KEY (name)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE = utf8mb4_bin COMMENT 'labels';

#----------------------------------------------------------------------------------
DROP TABLE IF EXISTS `items`;
CREATE TABLE `items`
(
    id           bigint         NOT NULL AUTO_INCREMENT PRIMARY KEY                              COMMENT '自增id',
    group_id     bigint         NOT NULL                                                         COMMENT 'group id',

    feature_id   varchar(64)    NOT NULL                                                         COMMENT '特征id',
    feature      text           NOT NULL                                                         COMMENT 'base64编码后的特征',
    image        mediumtext     NOT NULL                                                         COMMENT '原始图片',

    created_at   datetime       NOT NULL DEFAULT CURRENT_TIMESTAMP                               COMMENT '创建时间',
    updated_at   datetime       NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP   COMMENT '更新时间',
    KEY (group_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE = utf8mb4_bin COMMENT 'items';

#----------------------------------------------------------------------------------
DROP TABLE IF EXISTS `group_labels`;
CREATE TABLE `group_labels`
(
    group_id     bigint     NOT NULL                                                         COMMENT 'group id',
    label_id     bigint     NOT NULL                                                         COMMENT '标签id',

    created_at   datetime   NOT NULL DEFAULT CURRENT_TIMESTAMP                               COMMENT '创建时间',
    updated_at   datetime   NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP   COMMENT '更新时间',
    PRIMARY KEY (group_id, label_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE = utf8mb4_bin COMMENT 'group_labels';

