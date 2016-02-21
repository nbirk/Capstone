<?php

$postedData = json_decode(file_get_contents("php://input"));

$name = $postedData->game_name;
$url =  $postedData->image_url;
$description =  $postedData->game_description;
$platformId =  $postedData->game_pid;

add_game($name, $description, $url, $platformId);


function add_game($gameName, $gameDesc, $imgUrl, $pid)
{
	$results = array();
	$newId = '';
	$conn = oci_connect('malz', '1Qaz2wsx', '(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(Host=db1.chpc.ndsu.nodak.edu)(Port=1521)))(CONNECT_DATA=(SID=cs)))');
	
	$query = 'insert into Game (gameName, description, imageUrl, gameId) Values(:gameName, :gameDesc, :imgUrl, null) RETURNING gameId INTO :newId';

	$stid = oci_parse($conn,$query);
	oci_bind_by_name($stid, ':gameName', $gameName);
	oci_bind_by_name($stid, ':gameDesc', $gameDesc);
	oci_bind_by_name($stid, ':imgUrl', $imgUrl);
	oci_bind_by_name($stid, ':newId', $newId);
	$success = oci_execute($stid,OCI_COMMIT_ON_SUCCESS);
	
	oci_free_statement($stid);
	oci_close($conn);
	
	if($success)
	{
		add_platform($newId, $pid);
	}

}

function add_platform($gameId, $pid){
	$conn = oci_connect('malz', '1Qaz2wsx', '(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(Host=db1.chpc.ndsu.nodak.edu)(Port=1521)))(CONNECT_DATA=(SID=cs)))');

	
	$query = 'insert into GamePlatform (gameId, platformId) Values(:gameId, :pid)';
	$stid = oci_parse($conn,$query);
	
	oci_bind_by_name($stid, ':gameId', $gameId);
	oci_bind_by_name($stid, ':pid', $pid);
	$success = oci_execute($stid,OCI_COMMIT_ON_SUCCESS);
	
	oci_free_statement($stid);
	oci_close($conn);
	
	if($success)
	{
		echo $gameId;
	}
}


?>