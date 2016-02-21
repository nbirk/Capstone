<?php
// The connection string is loooooooong. It's easiest to copy/paste this line. Remember to replace 'username' and 'password'!
$conn = oci_connect('malz', '1Qaz2wsx', '(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(Host=db1.chpc.ndsu.nodak.edu)(Port=1521)))(CONNECT_DATA=(SID=cs)))');

//Select customer with last name from field
$query = 'select * from (select * from Review ORDER BY reviewId DESC) review2 Where rownum <= 3 ORDER BY rownum';

$stid = oci_parse($conn,$query);
oci_execute($stid,OCI_DEFAULT);

$results = array();
while ($row = oci_fetch_array($stid,OCI_ASSOC)) 
{
  $results[] = $row;
}
echo json_encode($results);
oci_free_statement($stid);
oci_close($conn);
?>