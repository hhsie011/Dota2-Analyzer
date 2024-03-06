// update ability
var express = require('express');
var router = express.Router();

const fs = require('fs');

router.post('/', function(req, res) {
  // outputs: request_type, limit, ability_id, upgrade_count
  var body = req.body.request_type + " " + req.body.ability_id + " " + req.body.upgrade_count + " 0 0 0 0 0 0 0 0 0 0 0 0";
  
  fs.writeFile('../backend/src/app/requests/requests.txt', body, err => {
    console.log(body);
    if (err) {
      console.error(err);
      return;
    }
    //file written successfully
  })
  res.end();
});

module.exports = router;
