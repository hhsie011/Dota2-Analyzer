// update match
var express = require('express');
var router = express.Router();

const fs = require('fs');

router.post('/', function(req, res) {
  // outputs: request_type, limit, match_id, win_type, first_blood_time, cluster_id, likes, dislikes
  var win_type = req.body.win_type == "Radiant" ? "True" : "False";
  var region = "";

  switch (req.body.cluster_region) {
    case "US WEST":
      region = "111";
      break;
    case "US EAST":
      region = "121";
      break;
    case "EUROPE":
      region = "131";
      break;
    case "JAPAN":
      region = "144";
      break;
    case "SINGAPORE":
      region = "151";
      break;
    case "DUBAI":
      region = "161";
      break;
    case "AUSTRALIA":
      region = "171";
      break;
    case "STOCKHOLM":
      region = "181";
      break;
    case "AUSTRIA":
      region = "191";
      break;
    case "BRAZIL":
      region = "201";
      break;
    case "SOUTHAFRICA":
      region = "211";
      break;
    case "PW TELECOM ZHEJIANG":
      region = "223";
      break;
    case "PW TELECOM SHANGHAI":
      region = "224";
      break;
    case "PW TELECOM GUANGDONG":
      region = "225";
      break;
    case "PW TELECOM WUHAN":
      region = "227";
      break;
    case "PW UNICOM":
      region = "231";
      break;
    case "PW UNICOM TIANJIN":
      region = "232";
      break;
    case "CHILE":
      region = "242";
      break;
    case "PERU":
      region = "251";
      break;
    case "INDIA":
      region = "261";
      break;
    default:
      region = "111";
      break;
  }

  var body = req.body.request_type + " " + req.body.match_id + " 0 0 0 0 0 0 " + req.body.first_blood_time + " 0 " + win_type + " " + req.body.likes + " " + req.body.dislikes + " " + region + " 0";

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
