<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.9" tiledversion="1.9.2" name="enemies" tilewidth="120" tileheight="60" tilecount="5" columns="0" objectalignment="bottomright">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" class="ENEMY">
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
     <property name="money" type="int" value="5"/>
     <property name="name" value="Cute Rat"/>
    </properties>
   </property>
  </properties>
  <image width="60" height="60" source="../images/cuterat.bmp"/>
 </tile>
 <tile id="1" class="ENEMY">
  <properties>
   <property name="STATS" type="class" propertytype="STAT">
    <properties>
     <property name="health" type="int" value="30"/>
     <property name="money" type="int" value="1"/>
     <property name="name" value="Beach Bald"/>
    </properties>
   </property>
   <property name="pop" type="class" propertytype="ATTACK">
    <properties>
     <property name="dam" type="int" value="-1"/>
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
  <image width="60" height="60" source="../images/beachbld.bmp"/>
 </tile>
 <tile id="2" class="ENEMY">
  <properties>
   <property name="BLOCKS YOUR DRIVEWAY" type="class" propertytype="ATTACK">
    <properties>
     <property name="dam" type="int" value="8"/>
     <property name="desc" value="%s %s!"/>
     <property name="prob" type="int" value="10"/>
    </properties>
   </property>
   <property name="LOSES YOUR MAIL" type="class" propertytype="ATTACK">
    <properties>
     <property name="dam" type="int" value="2"/>
     <property name="desc" value="%s %sES!"/>
     <property name="prob" type="int" value="30"/>
    </properties>
   </property>
   <property name="STATS" type="class" propertytype="STAT">
    <properties>
     <property name="health" type="int" value="150"/>
     <property name="money" type="int" value="8"/>
     <property name="name" value="Angry Mailman"/>
    </properties>
   </property>
  </properties>
  <image width="40" height="40" source="../images/madmail.bmp"/>
 </tile>
 <tile id="3" class="ENEMY">
  <properties>
   <property name="LOSES PACKETS" type="class" propertytype="ATTACK">
    <properties>
     <property name="dam" type="int" value="-8"/>
     <property name="desc" value="%s %s!"/>
     <property name="prob" type="int" value="70"/>
    </properties>
   </property>
   <property name="STATS" type="class" propertytype="STAT">
    <properties>
     <property name="health" type="int" value="90"/>
     <property name="money" type="int" value="10"/>
     <property name="name" value="Broken Router"/>
    </properties>
   </property>
   <property name="WARRENY IS EXPIRED" type="class" propertytype="ATTACK">
    <properties>
     <property name="dam" type="int" value="-10"/>
     <property name="desc" value="%ss %s!"/>
     <property name="prob" type="int" value="20"/>
    </properties>
   </property>
  </properties>
  <image width="40" height="40" source="../images/router.bmp"/>
 </tile>
 <tile id="4" class="ENEMY">
  <properties>
   <property name="EAT" type="class" propertytype="ATTACK">
    <properties>
     <property name="dam" type="int" value="-1"/>
     <property name="desc" value="%s %sS on your %s!"/>
     <property name="prob" type="int" value="10"/>
    </properties>
   </property>
   <property name="POOP" type="class" propertytype="ATTACK">
    <properties>
     <property name="dam" type="int" value="-20"/>
     <property name="desc" value="%s %sS on your head!"/>
     <property name="prob" type="int" value="20"/>
    </properties>
   </property>
   <property name="RETARD STRENGTH WIND MILL" type="class" propertytype="ATTACK">
    <properties>
     <property name="dam" type="int" value="-50"/>
     <property name="desc" value="%s performs %s!"/>
     <property name="prob" type="int" value="10"/>
    </properties>
   </property>
   <property name="SMACK" type="class" propertytype="ATTACK">
    <properties>
     <property name="dam" type="int" value="-15"/>
     <property name="desc" value="%s %sS you on your head!"/>
     <property name="prob" type="int" value="40"/>
    </properties>
   </property>
   <property name="STATS" type="class" propertytype="STAT">
    <properties>
     <property name="health" type="int" value="3000"/>
     <property name="item" type="int" propertytype="INV_LIST" value="2"/>
     <property name="money" type="int" value="100"/>
     <property name="name" value="Pot Belly Tard"/>
    </properties>
   </property>
  </properties>
  <image width="120" height="60" source="../images/potbelly.bmp"/>
 </tile>
</tileset>
