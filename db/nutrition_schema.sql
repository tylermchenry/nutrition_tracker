-- MySQL dump 10.13  Distrib 5.1.41, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: nutrition
-- ------------------------------------------------------
-- Server version	5.1.41-3ubuntu12

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `composite_food`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `composite_food` (
  `Composite_Id` int(11) NOT NULL AUTO_INCREMENT,
  `User_Id` int(11) NOT NULL,
  `Description` char(200) NOT NULL,
  `Weight_g` decimal(10,4) DEFAULT NULL,
  `Volume_floz` decimal(10,4) DEFAULT NULL,
  `Quantity` decimal(10,4) DEFAULT NULL,
  `Servings` decimal(10,4) DEFAULT NULL,
  PRIMARY KEY (`Composite_Id`) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `composite_food_link`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `composite_food_link` (
  `Composite_Id` int(11) NOT NULL,
  `Contained_Type` enum('Food','CompositeFood') NOT NULL,
  `Contained_Id` int(11) NOT NULL,
  `Magnitude` decimal(10,7) NOT NULL,
  `Unit` char(7) NOT NULL,
  PRIMARY KEY (`Composite_Id`,`Contained_Id`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Describes which individual foods are part of each compound f';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `data_source`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `data_source` (
  `DataSrc_ID` char(6) NOT NULL,
  `Authors` char(255) DEFAULT NULL,
  `Title` char(255) NOT NULL,
  `Year` char(4) DEFAULT NULL,
  `Journal` char(135) DEFAULT NULL,
  `Vol_City` char(16) DEFAULT NULL,
  `Issue_State` char(5) DEFAULT NULL,
  `Start_Page` char(5) DEFAULT NULL,
  `End_Page` char(5) DEFAULT NULL,
  PRIMARY KEY (`DataSrc_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `data_source_link`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `data_source_link` (
  `NDB_No` char(5) NOT NULL,
  `Nutr_No` char(3) NOT NULL,
  `DataSrc_ID` char(6) NOT NULL,
  PRIMARY KEY (`NDB_No`,`Nutr_No`,`DataSrc_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `derivation_code`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `derivation_code` (
  `Deriv_Cd` char(4) NOT NULL,
  `Deriv_Desc` char(120) NOT NULL,
  PRIMARY KEY (`Deriv_Cd`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `entry`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `entry` (
  `Entry_Id` int(11) NOT NULL AUTO_INCREMENT,
  `Date` date NOT NULL,
  `User_Id` int(11) NOT NULL,
  `Meal_Id` int(11) DEFAULT NULL,
  `Order` int(11) DEFAULT NULL,
  `Type` enum('Food','CompoundFood') NOT NULL,
  `Item_Id` int(11) NOT NULL,
  `Magnitude` decimal(10,4) NOT NULL,
  `Unit` char(7) NOT NULL,
  PRIMARY KEY (`Entry_Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `food_description`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `food_description` (
  `Food_Id` int(11) NOT NULL AUTO_INCREMENT,
  `User_Id` int(11) DEFAULT NULL,
  `Entry_Src` enum('USDA','Import','Custom') NOT NULL,
  `FdGrp_Cd` char(4) NOT NULL,
  `Long_Desc` char(200) DEFAULT NULL,
  `Shrt_Desc` char(60) DEFAULT NULL,
  `ComName` char(100) DEFAULT NULL,
  `ManufacName` char(65) DEFAULT NULL,
  `Survey` tinyint(1) DEFAULT NULL,
  `Ref_desc` char(135) DEFAULT NULL,
  `Refuse` decimal(2,0) unsigned DEFAULT NULL,
  `SciName` char(65) DEFAULT NULL,
  `N_Factor` decimal(4,2) unsigned DEFAULT NULL,
  `Pro_Factor` decimal(4,2) unsigned DEFAULT NULL,
  `Fat_Factor` decimal(4,2) unsigned DEFAULT NULL,
  `CHO_Factor` decimal(4,2) unsigned DEFAULT NULL,
  `Weight_g` decimal(10,4) unsigned DEFAULT NULL,
  `Volume_floz` decimal(10,4) unsigned DEFAULT NULL,
  `Quantity` decimal(10,4) unsigned DEFAULT NULL,
  `Servings` decimal(10,4) unsigned DEFAULT NULL,
  PRIMARY KEY (`Food_Id`) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=7540 DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `footnote`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `footnote` (
  `NDB_No` char(5) NOT NULL,
  `Footnt_No` char(4) NOT NULL,
  `Footnt_Typ` enum('D','M','N') NOT NULL,
  `Nutr_No` char(3) DEFAULT NULL,
  `Footnt_Txt` char(200) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `group_description`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_description` (
  `FdGrp_Cd` char(4) NOT NULL,
  `FdGrp_Desc` char(60) NOT NULL,
  PRIMARY KEY (`FdGrp_Cd`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meal`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meal` (
  `Meal_Id` int(11) NOT NULL AUTO_INCREMENT,
  `User_Id` int(11) NOT NULL,
  `Name` char(50) NOT NULL,
  PRIMARY KEY (`Meal_Id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `nutrient_data`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `nutrient_data` (
  `Food_Id` int(11) NOT NULL,
  `Nutr_No` char(3) NOT NULL,
  `Nutr_Val` decimal(10,3) unsigned NOT NULL,
  `Num_Data_Pts` decimal(5,0) unsigned NOT NULL,
  `Std_Error` decimal(8,3) DEFAULT NULL,
  `Src_Cd` char(2) NOT NULL,
  `Deriv_Cd` char(4) DEFAULT NULL,
  `Ref_NDB_No` char(5) DEFAULT NULL,
  `Add_Nutr_Mark` tinyint(1) unsigned DEFAULT NULL,
  `Num_Studies` decimal(2,0) unsigned DEFAULT NULL,
  `Min` decimal(10,3) unsigned DEFAULT NULL,
  `Max` decimal(10,3) unsigned DEFAULT NULL,
  `DF` decimal(2,0) unsigned DEFAULT NULL,
  `Low_EB` decimal(10,3) unsigned DEFAULT NULL,
  `Up_EB` decimal(10,3) unsigned DEFAULT NULL,
  `Stat_cmt` char(10) DEFAULT NULL,
  `CC` char(1) DEFAULT NULL,
  PRIMARY KEY (`Food_Id`,`Nutr_No`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `nutrient_definition`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `nutrient_definition` (
  `Nutr_No` char(3) NOT NULL,
  `Category` enum('Energy','Basic','Vitamin','Mineral','Additional','Hidden') NOT NULL DEFAULT 'Hidden',
  `ShortName` char(32) NOT NULL,
  `Units` char(7) NOT NULL,
  `Tagname` char(20) DEFAULT NULL,
  `NutrDesc` char(60) NOT NULL,
  `RDI` decimal(10,4) unsigned NOT NULL DEFAULT '0.0000',
  `Num_Dec` char(1) NOT NULL,
  `SR_Order` decimal(6,0) NOT NULL,
  `Disp_Order` decimal(6,0) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`Nutr_No`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `source_code`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `source_code` (
  `Src_Cd` char(2) NOT NULL,
  `SrcCd_Desc` char(60) NOT NULL,
  PRIMARY KEY (`Src_Cd`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary table structure for view `unit_description`
--

SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `unit_description` (
  `Unit` char(7),
  `Type` enum('Weight','Volume','Quantity','Serving','Energy','IU'),
  `Description` varchar(34)
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `units`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `units` (
  `Unit` char(7) NOT NULL,
  `Type` enum('Weight','Volume','Quantity','Serving','Energy','IU') NOT NULL,
  `Name` char(24) NOT NULL,
  `Factor` decimal(12,6) NOT NULL COMMENT 'Multiplication factor to get to grams (for weight) or fluid ounces (for volume)',
  PRIMARY KEY (`Unit`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `usda_ndb_link`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `usda_ndb_link` (
  `Food_Id` int(11) NOT NULL,
  `NDB_No` char(5) NOT NULL,
  PRIMARY KEY (`Food_Id`,`NDB_No`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `User_Id` int(11) NOT NULL AUTO_INCREMENT,
  `Name` char(50) NOT NULL,
  `PW_SHA1` char(40) NOT NULL,
  PRIMARY KEY (`User_Id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weight`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `weight` (
  `Food_Id` int(11) NOT NULL,
  `Seq` char(2) NOT NULL,
  `Amount` decimal(5,3) unsigned NOT NULL,
  `Msre_Desc` char(80) NOT NULL,
  `Gm_Wgt` decimal(7,1) unsigned NOT NULL,
  `Num_Data_Pts` decimal(3,0) unsigned DEFAULT NULL,
  `Std_Dev` decimal(7,3) unsigned DEFAULT NULL,
  PRIMARY KEY (`Food_Id`,`Seq`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Final view structure for view `unit_description`
--

/*!50001 DROP TABLE IF EXISTS `unit_description`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `unit_description` AS select `units`.`Unit` AS `Unit`,`units`.`Type` AS `Type`,concat(`units`.`Name`,' (',`units`.`Unit`,')') AS `Description` from `units` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2010-05-16 21:23:59
