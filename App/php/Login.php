<?php

$postedData = json_decode(file_get_contents("php://input"));
$username = $postedData->user_name;
$password = $postedData->pass_word;

login($username,$password);

function login($user, $pass)
{	// The connection string is loooooooong. It's easiest to copy/paste this line. Remember to replace 'username' and 'password'!
	$conn = oci_connect('malz', '1Qaz2wsx', '(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(Host=db1.chpc.ndsu.nodak.edu)(Port=1521)))(CONNECT_DATA=(SID=cs)))');
	
	//Select customer with last name from field
	$query = 'select accountId, userName from Account where userName = :user';
	$results = array();
	
	$user = '\\'.$user.'\\';
	$stid = oci_parse($conn,$query);
	oci_bind_by_name($stid, ':user', $user);
	//oci_bind_by_name($stid, ':pass', $pass);
	$success = oci_execute($stid,OCI_DEFAULT);

	while ($row = oci_fetch_array($stid,OCI_ASSOC)) 
	{
	  $results[] = $row;
	}


	echo json_encode($results);
	oci_free_statement($stid);
	oci_close($conn);
}

?>