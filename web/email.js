function sendemail_to_surfyn()
{
    var email = document.getElementById("email");
    var email_subject = document.getElementById("email-subject");
    var email_msg = document.getElementById("email-msg");


    var email_check = /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$/i;

    var email_feedback = document.getElementById("email-feedback");

    if( email_check.test(email.value))
    {
        email_feedback.innerHTML = "";
        email_feedback.style.color = "";
        email.className = "form-control";


        var email_service = 'https://surfyn.fr:7878/sendemailtosurfyn?';
        //var email_service = 'http://127.0.0.1:7878/sendemailtosurfyn?';
        email_service += "sender_email=";
        email_service += email.value;

        email_service += "&subject="
        email_service += email_subject.value;

        email_service += "&msg=";
        email_service += email_msg.value;

        fetch(email_service, { method: "POST"})
        .then(function(resp)
        {
          showSuccessSendEmailMessage();
          email.value = "";
          email_subject.value = "";
          email_msg.value = "";
        } )
        .catch(function(error) {

          console.log(error);
        });
      $("#sendemail").modal("hide");
    }
    else
    {
        if(email_check.test(email.value) == false)
        {
          email.className = "form-control is-invalid";
          email_feedback.innerHTML = "Merci de saisir une adresse e-mail valide!";
          email_feedback.style.color = "red";
          email_feedback.style.fontSize = "12px";
        }
        else
        {
          email_feedback.innerHTML = "";
          email_feedback.style.color = "";
          email.className = "form-control";
        }
        if( mot_de_passe.value == "")
        {
          mot_de_passe.className = "form-control is-invalid";
          pass_feedback.innerHTML = "Merci de saisir un mot de passe!";
          pass_feedback.style.color = "red";
          pass_feedback.style.fontSize = "12px";
        }
        else
        {
          pass_feedback.innerHTML = "";
          pass_feedback.style.color = "";
          mot_de_passe.className = "form-control";
        }
    }
}


function showSuccessSendEmailMessage()
{
     $('#sendemail-result').modal(
       {
         focus:true,
         show:true
       }
     );
     setTimeout(function() {
         $('#sendemail-result').modal('hide');
     }, 3000);
}
