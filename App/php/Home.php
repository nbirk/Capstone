<?php
// The connection string is loooooooong. It's easiest to copy/paste this line. Remember to replace 'username' and 'password'!
$conn = oci_connect('malz', '1Qaz2wsx', '(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(Host=db1.chpc.ndsu.nodak.edu)(Port=1521)))(CONNECT_DATA=(SID=cs)))');

$results = array();
$reviews = array();
$games = array();

//Select customer with last name from field
$query = 'select * from (select * from Review ORDER BY reviewId DESC) review2, Game, Account Where Game.gameId = review2.gameId and Account.accountid = review2.accountid and rownum <= 3 ORDER BY rownum';
$query2 = 'select * from (select gameId, AVG(rating) AS avg_rating from ListGame where rating IS NOT NULL Group By gameId) topGames, Game where topGames.gameId = Game.gameId Order By avg_rating DESC';

$stid = oci_parse($conn,$query);
$stid2 = oci_parse($conn,$query2);
oci_execute($stid,OCI_DEFAULT);

while ($row = oci_fetch_array($stid,OCI_ASSOC)) 
{
  $reviews[] = $row;
}
$results[] = $reviews;

oci_execute($stid2,OCI_DEFAULT);

while ($row = oci_fetch_array($stid2,OCI_ASSOC)) 
{
  $games[] = $row;
}
$results[] = $games;

echo json_encode($results);
oci_free_statement($stid);
oci_free_statement($stid2);
oci_close($conn);
?>