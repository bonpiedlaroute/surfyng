#!/usr/bin/env python
# -*- coding: utf-8 -*- 
# (c) Copyright 2021 
# author(s): Noel Tchidjo
# All rights reserved

template_email_msg_header_one = u"""\
<html lang="fr">
<head>
    <meta charset="utf-8">
    <style type="text/css">
    .ad_width{
      width:50%;
    }
@media only screen and (max-device-width: 1024px) {
  .ad_width{
    width:98%;
  }
	}
</style>
</head>
<body>
  <p> Bonjour @@USER_DISPLAY_NAME@@
  </p>
  <p>
    1 nouvelle annonce sur <a href="https://surfyn.fr" style="text-decoration:none;">Surfyn</a> correspond à vos critères de recherche immobilière.
  </p>
  <br/>
  <br/>
  """
template_email_msg_header_many = u"""\
<html lang="fr">
<head>
    <meta charset="utf-8">
    <style type="text/css">
    .ad_width{
      width:50%;
    }
@media only screen and (max-device-width: 1024px) {
  .ad_width{
    width:98%;
  }

	}
</style>
</head>
<body>
  <p> Bonjour @@USER_DISPLAY_NAME@@
  </p>
  <p>
    @@NB_NEW_AD@@ nouvelles annonces sur <a href="https://surfyn.fr" style="text-decoration:none;" >Surfyn</a> correspondent à vos critères de recherche immobilière.
  </p>
  <br/>
  <br/>
  """


template_email_msg_body = u"""\
  <a href="@@AD_URL@@" style="display:block;margin-bottom:25px;">
  <div class="ad_width" style="height:200px;border:2px solid #787878;">
    <div style="width:49%;height:100%;float:left;">
      <img style="width:100%;height:100%;" src="@@IMAGE@@" />
    </div>
    <div style="width:49%;height:100%;float:right;margin:auto 0;">
      <span style="font-size:16px;color: #787878;font-weight:bold;"> @@PROPERTY_TYPE@@
      </span>
      <br/><br/>
      <span style="font-size:16px;color: #787878;font-weight:bold;"> @@ROOMS@@ pièce(s) - @@SURFACE@@ m<sup>2</sup>
      </span>
      <br/><br/>
      <span style="font-size:16px;color: #787878;font-weight:bold;"> @@PRICE@@ €
      </span>
      <br/>
      <br/>
      <span style="font-size:16px;color: #787878;font-weight:bold;"> @@CITY@@
      </span>
    </div>
  </div>
  </a>
   """

template_email_msg_footer = u"""\
   <p>Vous souhaitez connaitre le prix d'un bien immobilier? vous pouvez estimer son prix en ligne, gratuitement sur <a href="https://surfyn.fr/estimation-immobiliere-en-ligne.html" style="text-decoration:none;" >Surfyn</a>
            <br/> <br/>A votre service<br/>L' équipe Surfyn
  </p>
</body>
</html>
   """
