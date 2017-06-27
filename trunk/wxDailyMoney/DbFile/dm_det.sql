/*
Navicat SQLite Data Transfer

Source Server         : t3
Source Server Version : 30714
Source Host           : :0

Target Server Type    : SQLite
Target Server Version : 30714
File Encoding         : 65001

Date: 2015-07-15 17:20:21
*/

PRAGMA foreign_keys = OFF;

-- ----------------------------
-- Table structure for dm_det
-- ----------------------------
DROP TABLE IF EXISTS "main"."dm_det";
CREATE TABLE dm_det (id_ INTEGER PRIMARY KEY, fr_ TEXT NOT NULL, frt_ TEXT NOT NULL, to_ TEXT NOT NULL, tot_ TEXT NOT NULL, dt_ INTEGER NOT NULL, mn_ REAL NOT NULL, ar_ INTEGER NOT NULL, nt_ TEXT);
