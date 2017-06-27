/*
Navicat SQLite Data Transfer

Source Server         : t3
Source Server Version : 30714
Source Host           : :0

Target Server Type    : SQLite
Target Server Version : 30714
File Encoding         : 65001

Date: 2015-07-15 17:20:14
*/

PRAGMA foreign_keys = OFF;

-- ----------------------------
-- Table structure for dm_acc
-- ----------------------------
DROP TABLE IF EXISTS "main"."dm_acc";
CREATE TABLE dm_acc (id_ TEXT PRIMARY KEY, nm_ TEXT NOT NULL, tp_ TEXT NOT NULL, ca_ INTEGER NULL, iv_ REAL NOT NULL);
