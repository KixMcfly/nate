<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.8" tiledversion="1.8.4" name="enemies" tilewidth="120" tileheight="60" tilecount="5" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" type="ENEMY">
  <properties>
   <property name="BITE" type="class" propertytype="ATTACK">
    <properties>
     <property name="dam" type="int" value="-5"/>
     <property name="desc" value="%s %sS you!"/>
     <property name="prob" type="int" value="45"/>
    </properties>
   </property>
   <property name="LICK" type="class" propertytype="ATTACK">
    <properties>
     <property name="dam" type="int" value="5"/>
     <property name="desc" value="%s %sS your face out of affection!"/>
     <property name="prob" type="int" value="45"/>
    </properties>
   </property>
   <property name="STAT" type="class" propertytype="STAT">
    <properties>
     <property name="health" type="int" value="100"/>
     <property name="item" type="int" propertytype="INV_LIST" value="0"/>
     <property name="money" type="int" value="5"/>
     <property name="name" value="Cute Rat"/>
    </properties>
   </property>
  </properties>
  <image width="60" height="60" source="../images/enemy/cuterat.bmp"/>
 </tile>
 <tile id="1" type="ENEMY">
  <properties>
   <property name="STAT" type="class" propertytype="STAT">
    <properties>
     <property name="health" type="int" value="30"/>
     <property name="money" type="int" value="1"/>
     <property name="name" value="Beach Bald"/>
    </properties>
   </property>
   <property name="pop" type="class" propertytype="ATTACK">
    <properties>
     <property name="dam" type="int" value="1"/>
     <property name="desc" value="%s %ss!"/>
     <property name="prob" type="int" value="20"/>
    </properties>
   </property>
   <property name="stare" type="class" propertytype="ATTACK">
    <properties>
     <property name="desc" value="%s %ss at you in your skimpy bikini!"/>
     <property name="prob" type="int" value="80"/>
    </properties>
   </property>
  </properties>
  <image width="60" height="60" source="../images/enemy/beachbld.bmp"/>
 </tile>
 <tile id="2" type="ENEMY">
  <image width="40" height="40" source="../images/enemy/madmail.bmp"/>
 </tile>
 <tile id="3" type="ENEMY">
  <image width="40" height="40" source="../images/enemy/router.bmp"/>
 </tile>
 <tile id="4" type="ENEMY">
  <properties>
   <property name="STATS" type="class" propertytype="STAT">
    <properties>
     <property name="health" type="int" value="3000"/>
     <property name="item" type="int" propertytype="INV_LIST" value="2"/>
     <property name="money" type="int" value="100"/>
     <property name="name" value="Pot Belly Tard"/>
    </properties>
   </property>
  </properties>
  <image width="120" height="60" source="../images/enemy/potbelly.bmp"/>
 </tile>
</tileset>
