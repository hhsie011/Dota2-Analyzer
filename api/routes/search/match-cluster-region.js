// search matches by cluster region
var express = require('express');
var router = express.Router();

const fs = require('fs');

router.post('/', function(req, res) {
  // outputs: request_type, limit
  var body = req.body.request_type + " " + req.body.limit;

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
