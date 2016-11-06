<!DOCTYPE html>
<html lang="fr">
<head>
   <link rel="stylesheet" href="styles.css">
   <title> Le bien de vos rêves </title>
   <meta charset="UTF-8">
</head>
<body>
<h1 class="title_results"> Surfyng
</h1>

<?php
error_reporting(E_ALL);
require_once '/usr/lib/php/Thrift/ClassLoader/ThriftClassLoader.php';
use Thrift\ClassLoader\ThriftClassLoader;
use Thrift\Protocol\TBinaryProtocol;
use Thrift\Transport\THttpClient;
use Thrift\Transport\TBufferedTransport;
use Thrift\Exception\TException;
use Thrift\Transport\TSocket;

require_once 'Sf_service.php';
require_once 'Types.php';


if(isset($_POST['valid']))
{
      

   $loader = new ThriftClassLoader();
   $loader->registerNamespace('Thrift','/usr/lib/php/');
   $loader->register();

   try
   {
      $socket = new TSocket('localhost', 9090);

      $transport = new TBufferedTransport($socket, 1024, 1024);
      $protocol = new TBinaryProtocol($transport);
      $client = new Sf_serviceClient($protocol);

      $transport->open();

      $properties = $client->getproperties();

      foreach($properties as $value)
      {
         echo "<div class=\"search_results\">";
         echo "<ul>";
         echo "<li>".$value->property_type."</li>";
         echo "<li>".$value->price."</li>";
         echo "<li>".$value->description."</li>";
         echo "<li>".$value->nb_rooms."</li>";
         echo "<li>".$value->url."</li>";
         echo "<li>".$value->images_path."</li>";
         echo "</ul>";
         echo "</div>";
      } 
      $transport->close();

   } 
   catch (TException $tx) 
   {
      print 'TException: '.$tx->getMessage()."\n";
   }
}
?>

<br>
</body>
</html>
