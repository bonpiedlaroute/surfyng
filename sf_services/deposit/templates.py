#!/usr/bin/env python
# -*- coding: utf-8 -*-
# (c) Copyright 2021
# author(s): Stephane Fedim
# All rights reserved

success_deposit_mail = u"""
<html lang="fr">

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <style type="text/css">
        .spacer-5 {
            height: 50px;
        }

        .spacer-2 {
            height: 20px;
        }

        .email-paragraph {
            font-size: 16px;
            line-height: 20px;
            font-weight: bold;
            color: #4C8E99;
            padding-bottom: 10px;
        }

        .email-btn {
            display: block;
            width: 25%;
            height: 20px;
            color: #fff !important;
            padding: 15px;
            font-size: 14px;
            text-align: center;
            border-radius: .25rem;
            font-weight: bold;
            background-color: #4C8E99;
            margin-left: auto;
            margin-right: auto;
        }

        a,
        a:hover,
        a:focus,
        a:active {
            text-decoration: none;
            color: white !important;
        }

        .offset {
            margin-left: 8.33%;
        }

        .morning_box {
            display: flex;
            width: 83.3%;
            background-color: #ccf8ff;
            border-radius: 10px;
            align-items: center !important;
            justify-content: center !important;
        }

        .hello_phrase {
            font-size: 28px;
            font-weight: bold;
            overflow-wrap: break-word;
        }

        .hello_online {
            font-size: 20px;
        }

        .hello_message {
            font-size: 14px;
        }

        .hello_period {
            font-size: 12px;
        }

        .morning_box_hello {
            width: 66.64%;
            margin-left: auto;
            margin-right: auto;
            text-align: center;
        }

        .morning_box_illustration {
            border-left: 2px solid white;
            text-align: center;
            padding-top: 10px;
            padding-bottom: 10px;
        }

        .announce_box {
            /* display: flex;
            flex-direction: column;
            align-items: center; */
            width: 83.33%;
            text-align: center;
        }

        .account_box {
            background-color: #ccf8ff;
            border-radius: 10px;
            text-align: center;
            width: 83.3%;
            padding-top: 10px;
            padding-bottom: 20px;
        }

        .footer_box {
            background-color: #222545;
            width: 79.90%;
            padding: 15px;
        }

        .img_dim {
            width: 75%;
            height: 160px;
        }

        @media only screen and (max-width: 720px) {
            .img_dim {
                width: 90%;
                height: 100px;
            }

            .email-btn {
                display: block;
                width: 40%;
                height: 20px;
                color: #fff !important;
                padding: 15px;
                font-size: 14px;
                text-align: center;
                border-radius: .25rem;
                font-weight: bold;
                background-color: #4C8E99;
                margin-left: auto;
                margin-right: auto;
            }
        }
    </style>
</head>

<body>
    <main>
        <div class="morning_box offset">
            <div class="morning_box_hello">
                <div style="text-align: center;">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Flogo_circle.png?alt=media&token=68ca544b-6c88-4095-a1ef-a039ddbbdaf9"
                        alt="logo">
                </div>

                <p class="hello_phrase">Bonjour @@display_name@@,</p>
                <strong class="hello_online">Votre annonce est en ligne !</strong>
                <p class="hello_message">
                    Votre annonce <strong>"@@ad_title@@"</strong> a été mise en ligne sur <a href="https://surfyn.fr"
                        style="color: #007bff!important;" rel="noopener noreferrer">surfyn.fr</a>.
                </p>

                <p class="hello_period">Elle restera sur le site pendant 2 mois, et est déjà visible.</p>
            </div>
            <!-- <div class="morning_box_illustration">
                <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Fillustration.png?alt=media&token=2fb0bed1-569f-473c-ba9a-a5fcf2865865"
                    alt="illustration" style="width: 60%;">
            </div> -->
        </div>

        <div class="spacer-5">
        </div>

        <div class="announce_box offset">
            <a class="email-btn" href="@@ad_link@@" target="_blank" rel="noopener noreferrer" role="button">
                Voir mon annonce
            </a>

            <div class="spacer-5">
            </div>

            <div class="offset" style="display: flex; width: 83.3%;">
                <div style="display: inline-block; width: 33.32%;"><img class="img_dim" src="@@image1@@"
                        style="display: block; border-radius: 4px; margin-left: auto; margin-right: auto;"
                        alt="image 1">
                </div>
                <div style="display: inline-block; width: 33.32%;"><img class="img_dim" src="@@image2@@"
                        style="display: block; border-radius: 4px; margin-left: auto; margin-right: auto;"
                        alt="image 2">
                </div>
                <div style="display: inlince-block; width: 33.32%;"><img class="img_dim" src="@@image3@@"
                        style="display: block; border-radius: 4px; margin-left: auto; margin-right: auto;"
                        alt="image 3">
                </div>
            </div>
        </div>

        <div class="spacer-2">
        </div>

        <div class="account_box offset">
            <p class="email-paragraph">
                A tout moment vous pouvez gérer l'ensemble de vos annonces <br> en ligne en vous connectant sur votre
                compte
                surfyn.
            </p>

            <a class="email-btn" href="https://surfyn.fr/mesrecherches.html" target="_blank" rel="noopener noreferrer"
                role="button">
                Voir mon compte surfyn
            </a>


        </div>
        <div class="spacer-5">

        </div>
        <div class="offset">
            <p class="email-paragraph" style="font-size: 20px;">Quelques astuces pour @@search_type@@ vite et bien !</p>

            <div class="spacer-2">
            </div>

            <p style="font-size: 16px; font-weight: bold;">Photos</p>
            <p style="font-size: 14px">Ajouter des photos et obtenez 7 fois plus de consultation sur votre annonce !
                <br>
                N'hésitez pas à les modifiez, changer l'ordre et attention à la lumière
            </p>

            <p style="font-size: 16px; font-weight: bold;">A la une</p>
            <p style="font-size: 14px">
                Mettez votre annonce en avant dans un emplacement spécial à droite des listes des annonces et <br>
                gagnez en visibilité!
            </p>

            <p style="font-size: 16px; font-weight: bold;">Logo urgent</p>
            <p style="font-size: 14px">
                Signalez aux acheteurs potentiels le caractère urgent de votre vente dans la liste de résultats et <br>
                vendez plus vite.
            </p>

            <p style="font-size: 16px; font-weight: bold;">A votre service. <br> L'équipe Surfyn.</p>
        </div>

        <div class="spacer-2">
        </div>

        <footer class="footer_box offset">
            <div style="text-align: center; border-bottom: 2px solid white;">
                <img style="height: 128px; width: 128px;"
                    src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Flogo_surfyn.png?alt=media&token=3e0925b3-0a34-464f-91b6-20345765bfdf"
                    alt="logo">
            </div>
            <div style="margin-top: 10px;">
                <a href="https://www.facebook.com/surfyn.fr/" target="_blank" rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Ffacebook_icon.png?alt=media&token=a507faa9-fbd8-43ba-aff3-6bdc6c345087"
                        alt="facebook_logo">
                </a>
                <a style="margin-left: 10px" href="https://twitter.com/SurfynImmo" target="_blank"
                    rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Ftwitter_icon.png?alt=media&token=e535ffc0-81ef-48d1-b99f-c2b4884e9356"
                        alt="twitter_logo">
                </a>
                <a style="margin-left: 10px" href="https://www.instagram.com/surfynimmo" target="_blank"
                    rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Finstagram_icon.png?alt=media&token=a843090c-f8fb-42a3-a2da-e72cfb815c46"
                        alt="instagram_logo">
                </a>
                <a style="margin-left: 10px" href="https://www.linkedin.com/company/surfyn" target="_blank"
                    rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Flinkedin_icon.png?alt=media&token=2751b675-b9f6-4289-bff1-f861c72d8ad0"
                        alt="linkedin_logo">
                </a>
                <a style="margin-left: 10px" href="https://www.youtube.com/channel/UCjFCLtdaDGtV4FgZzO3mh5A"
                    target="_blank" rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Fyoutube_icon.png?alt=media&token=5e1897c8-bb65-4de9-8a41-e6b22bd8fa4f"
                        alt="youtube_logo">
                </a>
            </div>
            <div class="spacer-2">
            </div>
            <!-- <div style="text-align: center; text-decoration: underline;">
                <a href="http://" target="_blank" rel="noopener noreferrer">Accéder à nos conditions générales
                    d'utilisation</a>
            </div> -->
        </footer>
    </main>
</body>

</html>
"""

success_deposit_mail_without_photos = u"""
<html lang="fr">

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <style type="text/css">
        .spacer-5 {
            height: 50px;
        }

        .spacer-2 {
            height: 20px;
        }

        .email-paragraph {
            font-size: 16px;
            line-height: 20px;
            font-weight: bold;
            color: #4C8E99;
            padding-bottom: 10px;
        }

        .email-btn {
            display: block;
            width: 25%;
            height: 20px;
            color: #fff !important;
            padding: 15px;
            font-size: 14px;
            text-align: center;
            border-radius: .25rem;
            font-weight: bold;
            background-color: #4C8E99;
            margin-left: auto;
            margin-right: auto;
        }

        a,
        a:hover,
        a:focus,
        a:active {
            text-decoration: none;
            color: white !important;
        }

        .offset {
            margin-left: 8.33%;
        }

        .morning_box {
            display: flex;
            width: 83.3%;
            background-color: #ccf8ff;
            border-radius: 10px;
            align-items: center !important;
            justify-content: center !important;
        }

        .hello_phrase {
            font-size: 28px;
            font-weight: bold;
            overflow-wrap: break-word;
        }

        .hello_online {
            font-size: 20px;
        }

        .hello_message {
            font-size: 14px;
        }

        .hello_period {
            font-size: 12px;
        }

        .morning_box_hello {
            width: 66.64%;
            margin-left: auto;
            margin-right: auto;
            text-align: center;
        }

        .morning_box_illustration {
            border-left: 2px solid white;
            text-align: center;
            padding-top: 10px;
            padding-bottom: 10px;
        }

        .announce_box {
            /* display: flex;
            flex-direction: column;
            align-items: center; */
            width: 83.33%;
            text-align: center;
        }

        .account_box {
            background-color: #ccf8ff;
            border-radius: 10px;
            text-align: center;
            width: 83.3%;
            padding-top: 10px;
            padding-bottom: 20px;
        }

        .footer_box {
            background-color: #222545;
            width: 79.90%;
            padding: 15px;
        }

        .img_dim {
            width: 75%;
            height: 160px;
        }

        @media only screen and (max-width: 720px) {
            .img_dim {
                width: 90%;
                height: 100px;
            }

            .email-btn {
                display: block;
                width: 40%;
                height: 20px;
                color: #fff !important;
                padding: 15px;
                font-size: 14px;
                text-align: center;
                border-radius: .25rem;
                font-weight: bold;
                background-color: #4C8E99;
                margin-left: auto;
                margin-right: auto;
            }
        }
    </style>
</head>

<body>
    <main>
        <div class="morning_box offset">
            <div class="morning_box_hello">
                <div style="text-align: center;">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Flogo_circle.png?alt=media&token=68ca544b-6c88-4095-a1ef-a039ddbbdaf9"
                        alt="logo">
                </div>

                <p class="hello_phrase">Bonjour @@display_name@@,</p>
                <strong class="hello_online">Votre annonce est en ligne !</strong>
                <p class="hello_message">
                    Votre annonce <strong>"@@ad_title@@"</strong> a été mise en ligne sur <a href="https://surfyn.fr"
                        style="color: #007bff!important;" rel="noopener noreferrer">surfyn.fr</a>.
                </p>

                <p class="hello_period">Elle restera sur le site pendant 2 mois, et est déjà visible.</p>
            </div>
            <!-- <div class="morning_box_illustration">
                <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Fillustration.png?alt=media&token=2fb0bed1-569f-473c-ba9a-a5fcf2865865"
                    alt="illustration" style="width: 60%;">
            </div> -->
        </div>

        <div class="spacer-5">
        </div>

        <div class="announce_box offset">
            <a class="email-btn" href="@@ad_link@@" target="_blank" rel="noopener noreferrer" role="button">
                Voir mon annonce
            </a>

            <div class="spacer-5">
            </div>

            <!-- <div class="offset" style="display: flex; width: 83.3%;">
                <div style="display: inline-block; width: 33.32%;"><img class="img_dim" src="@@image1@@"
                        style="display: block; border-radius: 4px; margin-left: auto; margin-right: auto;"
                        alt="image 1">
                </div>
                <div style="display: inline-block; width: 33.32%;"><img class="img_dim" src="@@image2@@"
                        style="display: block; border-radius: 4px; margin-left: auto; margin-right: auto;"
                        alt="image 2">
                </div>
                <div style="display: inlince-block; width: 33.32%;"><img class="img_dim" src="@@image3@@"
                        style="display: block; border-radius: 4px; margin-left: auto; margin-right: auto;"
                        alt="image 3">
                </div>
            </div> -->
        </div>

        <div class="spacer-2">
        </div>

        <div class="account_box offset">
            <p class="email-paragraph">
                A tout moment vous pouvez gérer l'ensemble de vos annonces <br> en ligne en vous connectant sur votre
                compte
                surfyn.
            </p>

            <a class="email-btn" href="https://surfyn.fr/mesrecherches.html" target="_blank" rel="noopener noreferrer"
                role="button">
                Voir mon compte surfyn
            </a>


        </div>
        <div class="spacer-5">

        </div>
        <div class="offset">
            <p class="email-paragraph" style="font-size: 20px;">Quelques astuces pour @@search_type@@ vite et bien !</p>

            <div class="spacer-2">
            </div>

            <p style="font-size: 16px; font-weight: bold;">Photos</p>
            <p style="font-size: 14px">Ajouter des photos et obtenez 7 fois plus de consultation sur votre annonce !
                <br>
                N'hésitez pas à les modifiez, changer l'ordre et attention à la lumière
            </p>

            <p style="font-size: 16px; font-weight: bold;">A la une</p>
            <p style="font-size: 14px">
                Mettez votre annonce en avant dans un emplacement spécial à droite des listes des annonces et <br>
                gagnez en visibilité!
            </p>

            <p style="font-size: 16px; font-weight: bold;">Logo urgent</p>
            <p style="font-size: 14px">
                Signalez aux acheteurs potentiels le caractère urgent de votre vente dans la liste de résultats et <br>
                vendez plus vite.
            </p>

            <p style="font-size: 16px; font-weight: bold;">A votre service. <br> L'équipe Surfyn.</p>
        </div>

        <div class="spacer-2">
        </div>

        <footer class="footer_box offset">
            <div style="text-align: center; border-bottom: 2px solid white;">
                <img style="height: 128px; width: 128px;"
                    src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Flogo_surfyn.png?alt=media&token=3e0925b3-0a34-464f-91b6-20345765bfdf"
                    alt="logo">
            </div>
            <div style="margin-top: 10px;">
                <a href="https://www.facebook.com/surfyn.fr/" target="_blank" rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Ffacebook_icon.png?alt=media&token=a507faa9-fbd8-43ba-aff3-6bdc6c345087"
                        alt="facebook_logo">
                </a>
                <a style="margin-left: 10px" href="https://twitter.com/SurfynImmo" target="_blank"
                    rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Ftwitter_icon.png?alt=media&token=e535ffc0-81ef-48d1-b99f-c2b4884e9356"
                        alt="twitter_logo">
                </a>
                <a style="margin-left: 10px" href="https://www.instagram.com/surfynimmo" target="_blank"
                    rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Finstagram_icon.png?alt=media&token=a843090c-f8fb-42a3-a2da-e72cfb815c46"
                        alt="instagram_logo">
                </a>
                <a style="margin-left: 10px" href="https://www.linkedin.com/company/surfyn" target="_blank"
                    rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Flinkedin_icon.png?alt=media&token=2751b675-b9f6-4289-bff1-f861c72d8ad0"
                        alt="linkedin_logo">
                </a>
                <a style="margin-left: 10px" href="https://www.youtube.com/channel/UCjFCLtdaDGtV4FgZzO3mh5A"
                    target="_blank" rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Fyoutube_icon.png?alt=media&token=5e1897c8-bb65-4de9-8a41-e6b22bd8fa4f"
                        alt="youtube_logo">
                </a>
            </div>
            <div class="spacer-2">
            </div>
            <!-- <div style="text-align: center; text-decoration: underline;">
                <a href="http://" target="_blank" rel="noopener noreferrer">Accéder à nos conditions générales
                    d'utilisation</a>
            </div> -->
        </footer>
    </main>
</body>

</html>
"""

deleting_deposit_mail = u"""
<html lang="fr">

<head>
    <meta charset="utf-8">
    <style type="text/css">
        .spacer-5 {
            height: 50px;
        }

        .spacer-2 {
            height: 20px;
        }

        .email-paragraph {
            font-size: 16px;
            line-height: 20px;
            /* font-weight: bold; */
            /* color: #4C8E99; */
            padding-bottom: 10px;
        }

        .email-btn {
            display: inline-block;
            width: 30%;
            /* height: 20px; */
            color: #fff !important;
            padding: 15px;
            font-size: 1rem;
            text-align: center;
            border-radius: .25rem;
            font-weight: bold;
            background-color: #4C8E99;
        }

        a,
        a:hover,
        a:focus,
        a:active {
            text-decoration: none;
            color: white !important;
        }

        .offset {
            margin-left: 8.33%;
        }

        .morning_box {
            display: flex;
            width: 83.3%;
            background-color: #ccf8ff;
            border-radius: 10px;
            align-items: center !important;
            justify-content: center !important;
            text-align: center;
        }

        .hello_phrase {
            font-size: 25px;
            font-weight: bold;
            overflow-wrap: break-word;
        }

        .hello_message {
            font-size: 14px;
        }

        .morning_box_hello {
            width: 100%;
            margin-left: 5%;
            margin-right: 5%;
            padding-top: 20px;
            padding-bottom: 20px;
            /* border-right: 2px solid white; */
        }

        .morning_box_illustration {
            /* border-left: 2px solid white; */
            text-align: center;
            /* padding-top: 50px; */
            /* padding-bottom: 50px; */
        }

        .announce_box {
            /* display: flex;
            flex-direction: column;
            align-items: center; */
            width: 83.33%;
            text-align: center;
        }

        .account_box {
            background-color: #ccf8ff;
            border-radius: 10px;
            text-align: center;
            width: 83.3%;
            padding-top: 10px;
            padding-bottom: 20px;
        }

        .footer_box {
            background-color: #222545;
            width: 83.3%;
            padding: 15px;
        }
    </style>
</head>

<body>
    <main>
        <div class="morning_box offset">
            <div class="morning_box_hello">
                <div style="text-align: center;">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Flogo_circle.png?alt=media&token=68ca544b-6c88-4095-a1ef-a039ddbbdaf9"
                    alt="logo">
                </div>
                
                <p class="hello_phrase">Bonjour @@display_name@@,</p>
                <p class="hello_message">
                    Votre annonce a bien été supprimée de <a href="https://surfyn.fr"
                        style="color: #007bff!important;" rel="noopener noreferrer">surfyn.fr</a> le @@date@@ à @@hour@@(UTC).
                </p>

            </div>
            <!-- <div class="morning_box_illustration">
                <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Fdeleting_illustration.png?alt=media&token=674efbd6-7eaa-4016-a3b4-ab8fd6700de1"
                    alt="illustration" style="width: 60%;">
            </div> -->
        </div>

        <div class="spacer-5">
        </div>

        <div class="account_box offset">
            <p class="email-paragraph">
                Il s'agit de l'annonce <span style="font-weight: bold;">"@@ad_title@@"</span>
                <br>
                <br>
                <br>
                Avez-vous une autre annonce à déposer?
            </p>

            <a class="email-btn" href="https://surfyn.fr/deposer-une-annonce.html" target="_blank" rel="noopener noreferrer"
                role="button">
                Déposer une nouvelle annonce
            </a>


        </div>
        <div class="spacer-5">

        </div>
        <div class="offset">
            <p style="font-size: 16px; font-weight: bold;">A votre service. <br> L'équipe Surfyn.</p>
        </div>

        <div class="spacer-2">
        </div>

        <footer class="footer_box offset">
            <div style="text-align: center; border-bottom: 2px solid white;">
                <img style="height: 128px; width: 128px;"
                    src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Flogo_surfyn.png?alt=media&token=3e0925b3-0a34-464f-91b6-20345765bfdf"
                    alt="logo">
            </div>
            <div style="margin-top: 10px;">
                <a href="https://www.facebook.com/surfyn.fr/" target="_blank" rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Ffacebook_icon.png?alt=media&token=a507faa9-fbd8-43ba-aff3-6bdc6c345087"
                        alt="facebook_logo">
                </a>
                <a style="margin-left: 10px" href="https://twitter.com/SurfynImmo" target="_blank"
                    rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Ftwitter_icon.png?alt=media&token=e535ffc0-81ef-48d1-b99f-c2b4884e9356"
                        alt="twitter_logo">
                </a>
                <a style="margin-left: 10px" href="https://www.instagram.com/surfynimmo" target="_blank"
                    rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Finstagram_icon.png?alt=media&token=a843090c-f8fb-42a3-a2da-e72cfb815c46"
                    alt="instagram_logo">
                </a>
                <a style="margin-left: 10px" href="https://www.linkedin.com/company/surfyn" target="_blank"
                    rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Flinkedin_icon.png?alt=media&token=2751b675-b9f6-4289-bff1-f861c72d8ad0"
                        alt="linkedin_logo">
                </a>
                <a style="margin-left: 10px" href="https://www.youtube.com/channel/UCjFCLtdaDGtV4FgZzO3mh5A"
                    target="_blank" rel="noopener noreferrer">
                    <img src="https://firebasestorage.googleapis.com/v0/b/surfyn.appspot.com/o/email%2Fyoutube_icon.png?alt=media&token=5e1897c8-bb65-4de9-8a41-e6b22bd8fa4f"
                        alt="youtube_logo">
                </a>
            </div>
            <div class="spacer-2">
            </div>
            <!-- <div style="text-align: center; text-decoration: underline;">
                <a href="http://" target="_blank" rel="noopener noreferrer">Accéder à nos conditions générales
                    d'utilisation</a>
            </div> -->
        </footer>
    </main>
</body>

</html>
"""
