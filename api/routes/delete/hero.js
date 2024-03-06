// delete hero
var express = require('express');
var router = express.Router();

const fs = require('fs');

router.post('/', function(req, res) {
  // outputs: request_type, hero_id
  var body = req.body.request_type + " " + req.body.hero_id + " 0 0 0 0 0 0 0 0 0 0 0 0 0";
  
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
