<html>
<body>

Independence Day Flag Generated!
<br>
<br><br>

<img src="flag.gif" />

<?php
$vIday = $_POST["Iday"];
$vCflag1 = boolval($_POST["Cflag1"]) & 1;
$vCflag2 = boolval($_POST["Cflag2"]) & 1;
$vCflag3 = boolval($_POST["Cflag3"]) & 1;
$vCflag4 = boolval($_POST["Cflag4"]) & 1;
$vflagSize = $_POST["flagSize"];

$binhash = $vIday . $vCflag1 . $vCflag2 . $vCflag3 . $vCflag4 . $vflagSize;

$shellCommand = "sudo python /home/pi/html/flagWave.py " . $binhash;

$output = shell_exec($shellCommand);

?>

</body>
</html>

