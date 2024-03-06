import React, { Component } from "react";
import axios from "axios";
import Container from "../components/Container"
import styles from "../styles/Common.module.css"
import Matchstyles from "../styles/Matches.module.css";
import Modalstyles from "../styles/Modal.module.css";

import Modal from "../components/Modal";
import Match from "../components/Match";
import Match2 from "../components/Match2";
import Match3 from "../components/Match3";

import { GiSwordsEmblem } from "react-icons/gi";
import { MdCloudUpload, MdCreate, MdClear, MdHistory } from "react-icons/md";
import { CgFormatRight } from "react-icons/cg";

const api = axios.create({ baseURL: 'http://localhost:8080/'});

export default class MatchesPage extends Component {
  constructor() {
    super();
    this.state = {
      activeMatch: 1,
      createFormOpenFB: false,
      createFormOpenCR: false,
      createFormOpenML: false,
      prevSearchFB: 0,
      prevSearchCR: 0,
      prevSearchML: 0,
      matchID: 0,
      winType: "",
      firstBloodTime: 0,
      clusterRegion: "US WEST",
      likes: 0,
      dislikes: 0,
      backupFormOpen: false,
      importFormOpen: false
    }
    this.importSuccess = false;
    this.activateFirstBloodTime = this.activateFirstBloodTime.bind(this);
    this.activateClusterRegion = this.activateClusterRegion.bind(this);
    this.activateMostLiked = this.activateMostLiked.bind(this);
    this.toggleCreateFormFB = this.toggleCreateFormFB.bind(this);
    this.toggleCreateFormCR = this.toggleCreateFormCR.bind(this);
    this.toggleCreateFormML = this.toggleCreateFormML.bind(this);
    this.handleChangeMatchID = this.handleChangeMatchID.bind(this);
    this.handleChangeWinType = this.handleChangeWinType.bind(this);
    this.handleChangeFBTime = this.handleChangeFBTime.bind(this);
    this.handleChangeClusterRegion = this.handleChangeClusterRegion.bind(this);
    this.handleChangeLikes = this.handleChangeLikes.bind(this);
    this.handleChangeDislikes = this.handleChangeDislikes.bind(this);
    this.insertMatch = this.insertMatch.bind(this);
    this.import = this.import.bind(this);
    this.updateSearchAmount = this.updateSearchAmount.bind(this);
  }
  
  MatchForm = (selectID) => {
    const { matchID, winType, firstBloodTime, likes, dislikes } = this.state;
    return (
      <form 
        className={styles.formContent}
        onSubmit={this.insertMatch}
      >
        <div className={styles.formSection}>
          <div className={styles.sectionTitle}>
            MATCH ID
          </div>
          <input required
            type="text" 
            pattern="[0-9]+"
            title="Input only numbers."
            value={matchID}
            onChange={this.handleChangeMatchID}
          />
        </div>
        <div className={styles.formSection}>
          <div className={styles.sectionTitle}>
            WIN TYPE 
          </div>
          <input required
            type="text" 
            pattern="(Radiant)||(Dire)"
            title={"Input either \"Radiant\" or \"Dire.\""}
            value={winType}
            onChange={this.handleChangeWinType}
          />
        </div>
        <div className={styles.formSection}>
          <div className={styles.sectionTitle}>
            FIRST BLOOD TIME
          </div>
          <input required
            type="text" 
            pattern="[0-9]+"
            title="Input only numbers."
            value={firstBloodTime}
            onChange={this.handleChangeFBTime}
          />
        </div>
        <div className={styles.formSection}>
          <div className={styles.sectionTitle}>
            CLUSTER REGION
          </div>
          <select 
            onChange={this.handleChangeClusterRegion}
            id={selectID}
          >
            <option value="US WEST">US WEST</option>
            <option value="US EAST">US EAST</option>
            <option value="EUROPE">EUROPE</option>
            <option value="JAPAN">JAPAN</option>
            <option value="SINGAPORE">SINGAPORE</option>
            <option value="DUBAI">DUBAI</option>
            <option value="AUSTRALIA">AUSTRALIA</option>
            <option value="STOCKHOLM">STOCKHOLM</option>
            <option value="AUSTRIA">AUSTRIA</option>
            <option value="BRAZIL">BRAZIL</option>
            <option value="SOUTHAFRICA">SOUTH AFRICA</option>
            <option value="PW TELECOM ZHEJIANG">PW TELECOM ZHEJIANG</option>
            <option value="PW TELECOM SHANGHAI">PW TELECOM SHANGHAI</option>
            <option value="PW TELECOM GUANGDONG">PW TELECOM GUANGDONG</option>
            <option value="PW TELECOM WUHAN">PW TELECOM WUHAN</option>
            <option value="PW UNICOM">PW UNICOM</option>
            <option value="PW UNICOM TIANJIN">PW UNICOM TIANJIN</option>
            <option value="CHILE">CHILE</option>
            <option value="PERU">PERU</option>
            <option value="INDIA">INDIA</option>
          </select>
        </div>
        <div className={styles.formSection}>
          <div className={styles.sectionTitle}>
            LIKE COUNT
          </div>
          <input required
            type="text" 
            pattern="[0-9]+"
            title="Input only numbers."
            value={likes}
            onChange={this.handleChangeLikes}
          />
        </div>
        <div className={styles.formSection}>
          <div className={styles.sectionTitle}>
            DISLIKE COUNT
          </div>
          <input required
            type="text" 
            pattern="[0-9]+"
            title="Input only numbers."
            value={dislikes}
            onChange={this.handleChangeDislikes}
          />
        </div>
        <input type="submit" value="CREATE NEW MATCH"></input>
      </form>
    );
  }

  componentDidMount() {
    var savedActive = Number(sessionStorage.getItem('savedActive'));
    this.setState({ activeMatch: savedActive ? savedActive : this.state.activeMatch });
    
    var searchFB = Number(sessionStorage.getItem('searchFB'));
    this.setState({ prevSearchFB: searchFB ? searchFB : 25 });

    var searchCR = Number(sessionStorage.getItem('searchCR'));
    this.setState({ prevSearchCR: searchCR ? searchCR : 25 });

    var searchML = Number(sessionStorage.getItem('searchML'));
    this.setState({ prevSearchML: searchML ? searchML : 25 });

    if (savedActive === 1) {
      const req = JSON.stringify ({
        "request_type": "search_first_blood_time",
        "limit": searchFB
      })
      api.post('/search/match/first-blood', req, 
        {headers: {"content-type": "application/json"}})
        .then(res => { console.log(res.data); })
        .catch(err => { console.log(err); })
    }
    else if (savedActive === 2) {
      const req = JSON.stringify ({
        "request_type": "search_cluster_region",
        "limit": searchCR
      })
      api.post('/search/match/cluster-region', req, 
        {headers: {"content-type": "application/json"}})
        .then(res => { console.log(res.data); })
        .catch(err => { console.log(err); })
    }
    else if (savedActive === 3) {
      const req = JSON.stringify ({
        "request_type": "search_most_liked_match",
        "limit": searchML
      })
      api.post('/search/match/most-likes', req, 
        {headers: {"content-type": "application/json"}})
        .then(res => { console.log(res.data); })
        .catch(err => { console.log(err); })
    }
  }

  componentDidUpdate(){
    sessionStorage.setItem('savedActive', this.state.activeMatch);
    sessionStorage.setItem('searchFB', this.state.prevSearchFB);
    sessionStorage.setItem('searchCR', this.state.prevSearchCR);
    sessionStorage.setItem('searchML', this.state.prevSearchML);
  }

  componentWillUnmount(){
    sessionStorage.setItem('savedActive', this.state.activeMatch);
    sessionStorage.setItem('searchFB', this.state.prevSearchFB);
    sessionStorage.setItem('searchCR', this.state.prevSearchCR);
    sessionStorage.setItem('searchML', this.state.prevSearchML);
  }

  activateFirstBloodTime() {
    if (this.state.activeMatch !== 1) {
      const searchFB = Number(sessionStorage.getItem('searchFB'));
      const req = JSON.stringify ({
        "request_type": "search_first_blood_time",
        "limit": searchFB
      })
      api.post('/search/match/first-blood', req, 
        {headers: {"content-type": "application/json"}})
        .then(res => { console.log(res.data); })
        .catch(err => { console.log(err); })
    }
    this.setState({ 
      activeMatch: 1, 
      createFormOpenFB: false,
      createFormOpenCR: false,
      createFormOpenML: false,
      matchID: 0,
      winType: "",
      firstBloodTime: 0,
      clusterRegion: "US WEST",
      likes: 0,
      dislikes: 0  
    });
  }

  activateClusterRegion() {
    if (this.state.activeMatch !== 2) {
      const searchCR = Number(sessionStorage.getItem('searchCR'));
      const req = JSON.stringify ({
        "request_type": "search_cluster_region",
        "limit": searchCR
      })
      api.post('/search/match/cluster-region', req, 
        {headers: {"content-type": "application/json"}})
        .then(res => { console.log(res.data); })
        .catch(err => { console.log(err); })
    }
    this.setState({ 
      activeMatch: 2, 
      createFormOpenFB: false,
      createFormOpenCR: false,
      createFormOpenML: false,
      matchID: 0,
      winType: "",
      firstBloodTime: 0,
      clusterRegion: "US WEST",
      likes: 0,
      dislikes: 0  
    });
  }

  activateMostLiked() {
    if (this.state.activeMatch !== 3) {
      const searchML = Number(sessionStorage.getItem('searchML'));
      const req = JSON.stringify ({
        "request_type": "search_most_liked_match",
        "limit": searchML
      })
      api.post('/search/match/most-likes', req, 
        {headers: {"content-type": "application/json"}})
        .then(res => { console.log(res.data); })
        .catch(err => { console.log(err); })
    }
    this.setState({ 
      activeMatch: 3, 
      createFormOpenFB: false,
      createFormOpenCR: false,
      createFormOpenML: false,
      matchID: 0,
      winType: "",
      firstBloodTime: 0,
      clusterRegion: "US WEST",
      likes: 0,
      dislikes: 0  
    });
  }

  toggleCreateFormFB() {
    if (this.state.createFormOpenFB) { document.getElementById("clusters1").value = "US WEST"; }
    this.state.createFormOpenFB ? 
      this.setState({ 
        createFormOpenFB: false,
        matchID: 0,
        winType: "",
        firstBloodTime: 0,
        clusterRegion: "US WEST",
        likes: 0,
        dislikes: 0 
      })
      : this.setState({ createFormOpenFB: true });
  }
  toggleCreateFormCR() {
    if (this.state.createFormOpenCR) { document.getElementById("clusters2").value = "US WEST"; }
    this.state.createFormOpenCR ? 
      this.setState({ 
        createFormOpenCR: false,
        matchID: 0,
        winType: "",
        firstBloodTime: 0,
        clusterRegion: "US WEST",
        likes: 0,
        dislikes: 0 
      })
      : this.setState({ createFormOpenCR: true });
  }
  toggleCreateFormML() {
    if (this.state.createFormOpenML) { document.getElementById("clusters3").value = "US WEST"; }
    this.state.createFormOpenML ? 
      this.setState({ 
        createFormOpenML: false,
        matchID: 0,
        winType: "",
        firstBloodTime: 0,
        clusterRegion: "US WEST",
        likes: 0,
        dislikes: 0 
      })
      : this.setState({ createFormOpenML: true });
  }

  handleChangeMatchID(e) {
    this.setState({ matchID: e.target.value });
  }
  handleChangeWinType(e) {
    this.setState({ winType: e.target.value });
  }
  handleChangeFBTime(e) {
    this.setState({ firstBloodTime: e.target.value });
  }
  handleChangeClusterRegion(e) {
    this.setState({ clusterRegion: e.target.value });
  }
  handleChangeLikes(e) {
    this.setState({ likes: e.target.value });
  }
  handleChangeDislikes(e) {
    this.setState({ dislikes: e.target.value });
  }

  insertMatch(e) {
    e.preventDefault();
    const { matchID,  winType, firstBloodTime, clusterRegion, likes, dislikes } = this.state;
    const req = JSON.stringify({
      "request_type": "insert_match",
      "match_id": matchID,
      "win_type": winType,
      "first_blood_time": firstBloodTime,
      "cluster_region": clusterRegion,
      "likes": likes,
      "dislikes": dislikes
    });
    api.post('/insert/match', req, 
      {headers: {"content-type": "application/json"}})
      .then(res => { console.log(res) })
      .catch(err => { console.log(err) })
  }

  openBackup = () => {
    this.setState({ backupFormOpen: !this.state.backupFormOpen });
  }

  openImport = () => {
    this.setState({ importFormOpen: !this.state.importFormOpen });
  }

  updateSearchAmount(amount) {
    if (this.state.activeMatch === 1) {
      sessionStorage.setItem('searchFB', amount);
      if (this.state.prevSearchFB !== amount) {
        const req = JSON.stringify ({
          "request_type": "search_first_blood_time",
          "limit": amount
        })
        api.post('/search/match/first-blood', req, 
          {headers: {"content-type": "application/json"}})
          .then(res => { console.log(res.data); })
          .catch(err => { console.log(err); })
      }
      this.setState({ prevSearchFB: amount });
    }
    else if (this.state.activeMatch === 2) {
      sessionStorage.setItem('searchCR', amount);
      if (this.state.prevSearchCR !== amount) {
        const req = JSON.stringify ({
          "request_type": "search_cluster_region",
          "limit": amount
        })
        api.post('/search/match/cluster-region', req, 
          {headers: {"content-type": "application/json"}})
          .then(res => { console.log(res.data); })
          .catch(err => { console.log(err); })
      }
      this.setState({ prevSearchCR: amount });
    }
    else if (this.state.activeMatch === 3) {
      sessionStorage.setItem('searchML', amount);
      if (this.state.prevSearchML !== amount) {
        const req = JSON.stringify ({
          "request_type": "search_most_liked_match",
          "limit": amount
        })
        api.post('/search/match/most-likes', req, 
          {headers: {"content-type": "application/json"}})
          .then(res => { console.log(res.data); })
          .catch(err => { console.log(err); })
      }
      this.setState({ prevSearchML: amount });
    }
  }

  backup() {
    const req = JSON.stringify({
      "request_type": "backup_match"
    });
    api.post('/backup/common', req, 
      {headers: {"content-type": "application/json"}})
      .then(res => { console.log(res) })
      .catch(err => { console.log(err) })
  }

  import() {
    axios.get('./../backupResults.json')
      .then(res => {
        console.log(res);
        this.importSuccess = res.data.results[0].result;
      })
    
    if (this.importSuccess) {
      const req = JSON.stringify({
        "request_type": "import_match"
      });
      api.post('/import/common', req, 
        {headers: {"content-type": "application/json"}})
        .then(res => { console.log(res) })
        .catch(err => { console.log(err) })
    }
  }

  render() {
    var searchFB = Number(sessionStorage.getItem('searchFB'));
    searchFB = searchFB ? searchFB : 25;
    var searchCR = Number(sessionStorage.getItem('searchCR'));
    searchCR = searchCR ? searchCR : 25;
    var searchML = Number(sessionStorage.getItem('searchML'));
    searchML = searchML ? searchML : 25;

    const { activeMatch, createFormOpenFB, createFormOpenCR, createFormOpenML, backupFormOpen, importFormOpen } = this.state;

    return (
      <Container main>
        <div className={styles.container}>
          <div className={styles.wrapper}>
            <div className={styles.choiceWrapper}>
              <h1 className={`${styles.title} ${styles.imageTextWrapper}`}>
                <GiSwordsEmblem /> 
                <div className={styles.imageText}>Matches</div>
              </h1>
              <div className={styles.buttonWrapper}>
                <div 
                  onClick={this.activateFirstBloodTime} 
                  className={activeMatch === 1 ? `${styles.button} ${styles.activeButton}` : `${styles.button}`}
                >
                  First Blood Time
                </div>
                <div 
                  onClick={this.activateClusterRegion} 
                  className={activeMatch === 2 ? `${styles.button} ${styles.activeButton}` : `${styles.button}`}
                >
                  Cluster Region
                </div>
                <div 
                  onClick={this.activateMostLiked} 
                  className={activeMatch === 3 ? `${styles.button} ${styles.activeButton}` : `${styles.button}`}
                >
                  Most Liked
                </div>
              </div>
            </div>

            {/* Matches by First Blood Time */}
            <div className={activeMatch === 1 ? `${styles.buttonContainer}` : `${styles.buttonContainer} ${styles.hidden}`}>
              <div className={styles.buttonWrapper}>
                <div 
                  onClick={this.toggleCreateFormFB}
                  className={createFormOpenFB ? `${styles.button} ${styles.toggleButton}` : `${styles.button}`}
                >
                  <div className={createFormOpenFB ? `${styles.buttonIcon} ${styles.hidden}` : `${styles.buttonIcon}`}><MdCreate /></div>
                  <div className={createFormOpenFB ? `${styles.buttonIcon}` : `${styles.buttonIcon} ${styles.hidden}`}><MdClear /></div>
                </div>
                <div 
                  onClick={() => this.openBackup()}
                  className={styles.button}
                >
                  <div className={styles.buttonIcon}><MdCloudUpload /></div>
                </div>
                <div 
                  onClick={() => this.openImport()}
                  className={styles.button}
                >
                  <div className={styles.buttonIcon}><MdHistory /></div>
                </div>
              </div>
              <div className={styles.buttonStack}>
                <div className={`${styles.buttonHeader} ${styles.buttonIcon}`}>
                  SEARCH LIMIT
                </div>
                <div className={styles.buttonWrapper}>
                  <div 
                    onClick={() => this.updateSearchAmount(25)}
                    className={searchFB === 25 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      25
                    </div>
                  </div>
                  <div 
                    onClick={() => this.updateSearchAmount(50)}
                    className={searchFB === 50 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      50
                    </div>
                  </div>
                  <div 
                    onClick={() => this.updateSearchAmount(100)}
                    className={searchFB === 100 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      100
                    </div>
                  </div>
                </div>
              </div>
            </div>

            <div className={createFormOpenFB ? `${styles.formContainer} ${Matchstyles.revealForm}` : `${styles.formContainer}` && activeMatch !== 1 ? `${styles.hideFormImmediate} ${styles.formContainer}` : `${styles.formContainer}`}>
              <div className={styles.formFiller}></div>
              <div className={createFormOpenFB ? `${styles.contentWrapper} ${Matchstyles.revealContent}` : `${styles.contentWrapper}` && activeMatch !== 1 ? `${styles.hideContentImmediate} ${styles.contentWrapper}` : `${styles.contentWrapper}`}>
                <div className={createFormOpenFB ? `${styles.formWrapper}` : `${styles.formWrapper} ${styles.hidden}`}>
                  <div className={styles.formTitle}>
                    Create New Match
                  </div>
                  {this.MatchForm("clusters1")}
                </div>
                <div className={createFormOpenFB ? `${styles.formFillerIcon}` : `${styles.formFillerIcon} ${styles.hidden}`}>
                  <CgFormatRight />
                </div>
              </div>
            </div>
            <div 
              className={activeMatch === 1 ? `${styles.table} ${styles.activeTable}` : `${styles.matchTable}`}
            >
              <div className={styles.tableHeader}>
                <div className={Matchstyles.fbColumnOne}>
                  Rank
                </div>
                <div className={Matchstyles.fbColumnTwo}>
                  Match ID
                </div>
                <div className={Matchstyles.fbColumnThree}>
                  Win Type
                </div>
                <div className={Matchstyles.fbColumnFour}>
                  First Blood Time (s)
                </div>
              </div>
              <Match />
            </div>

            {/* Matches by Cluster Region */}
            <div className={activeMatch === 2 ? `${styles.buttonContainer}` : `${styles.buttonContainer} ${styles.hidden}`}>
              <div className={styles.buttonWrapper}>
                <div 
                  onClick={this.toggleCreateFormCR}
                  className={createFormOpenCR ? `${styles.button} ${styles.toggleButton}` : `${styles.button}`}
                >
                  <div className={createFormOpenCR ? `${styles.buttonIcon} ${styles.hidden}` : `${styles.buttonIcon}`}><MdCreate /></div>
                  <div className={createFormOpenCR ? `${styles.buttonIcon}` : `${styles.buttonIcon} ${styles.hidden}`}><MdClear /></div>
                </div>
                <div 
                  onClick={() => this.openBackup()}
                  className={styles.button}
                >
                  <div className={styles.buttonIcon}><MdCloudUpload /></div>
                </div>
                <div 
                  onClick={() => this.openImport()}
                  className={styles.button}
                >
                  <div className={styles.buttonIcon}><MdHistory /></div>
                </div>
              </div>
              <div className={styles.buttonStack}>
                <div className={`${styles.buttonHeader} ${styles.buttonIcon}`}>
                  SEARCH LIMIT
                </div>
                <div className={styles.buttonWrapper}>
                  <div 
                    onClick={() => this.updateSearchAmount(25)}
                    className={searchCR === 25 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      25
                    </div>
                  </div>
                  <div 
                    onClick={() => this.updateSearchAmount(50)}
                    className={searchCR === 50 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      50
                    </div>
                  </div>
                  <div 
                    onClick={() => this.updateSearchAmount(100)}
                    className={searchCR === 100 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      100
                    </div>
                  </div>
                </div>
              </div>
            </div>

            <div className={createFormOpenCR ? `${styles.formContainer} ${Matchstyles.revealForm}` : `${styles.formContainer}` && activeMatch !== 2 ? `${styles.hideFormImmediate} ${styles.formContainer}` : `${styles.formContainer}`}>
              <div className={styles.formFiller}></div>
              <div className={createFormOpenCR ? `${styles.contentWrapper} ${Matchstyles.revealContent}` : `${styles.contentWrapper}` && activeMatch !== 2 ? `${styles.hideContentImmediate} ${styles.contentWrapper}` : `${styles.contentWrapper}`}>
                <div className={createFormOpenCR ? `${styles.formWrapper}` : `${styles.formWrapper} ${styles.hidden}`}>
                  <div className={styles.formTitle}>
                    Create New Match
                  </div>
                  {this.MatchForm("clusters2")}
                </div>
                <div className={createFormOpenCR ? `${styles.formFillerIcon}` : `${styles.formFillerIcon} ${styles.hidden}`}>
                  <CgFormatRight />
                </div>
              </div>
            </div>
            <div 
              className={activeMatch === 2 ? `${styles.table} ${styles.activeTable}` : `${styles.matchTable}`}
            >
              <div className={styles.tableHeader}>
                <div className={Matchstyles.crColumnOne}>
                  Rank
                </div>
                <div className={Matchstyles.crColumnTwo}>
                  Match ID
                </div>
                <div className={Matchstyles.crColumnThree}>
                  Cluster ID
                </div>
                <div className={Matchstyles.crColumnFour}>
                  Region
                </div>
              </div>
              <Match2 />
            </div>

            {/* Most Liked Matches */}
            <div className={activeMatch === 3 ? `${styles.buttonContainer}` : `${styles.buttonContainer} ${styles.hidden}`}>
              <div className={styles.buttonWrapper}>
                <div 
                  onClick={this.toggleCreateFormML}
                  className={createFormOpenML ? `${styles.button} ${styles.toggleButton}` : `${styles.button}`}
                >
                  <div className={createFormOpenML ? `${styles.buttonIcon} ${styles.hidden}` : `${styles.buttonIcon}`}><MdCreate /></div>
                  <div className={createFormOpenML ? `${styles.buttonIcon}` : `${styles.buttonIcon} ${styles.hidden}`}><MdClear /></div>
                </div>
                <div 
                  onClick={() => this.openBackup()}
                  className={styles.button}
                >
                  <div className={styles.buttonIcon}><MdCloudUpload /></div>
                </div>
                <div 
                  onClick={() => this.openImport()}
                  className={styles.button}
                >
                  <div className={styles.buttonIcon}><MdHistory /></div>
                </div>
              </div>
              <div className={styles.buttonStack}>
                <div className={`${styles.buttonHeader} ${styles.buttonIcon}`}>
                  SEARCH LIMIT
                </div>
                <div className={styles.buttonWrapper}>
                  <div 
                    onClick={() => this.updateSearchAmount(25)}
                    className={searchML === 25 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      25
                    </div>
                  </div>
                  <div 
                    onClick={() => this.updateSearchAmount(50)}
                    className={searchML === 50 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      50
                    </div>
                  </div>
                  <div 
                    onClick={() => this.updateSearchAmount(100)}
                    className={searchML === 100 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      100
                    </div>
                  </div>
                </div>
              </div>
            </div>

            <div className={createFormOpenML ? `${styles.formContainer} ${Matchstyles.revealForm}` : `${styles.formContainer}` && activeMatch !== 3 ? `${styles.hideFormImmediate} ${styles.formContainer}` : `${styles.formContainer}`}>
              <div className={styles.formFiller}></div>
              <div className={createFormOpenML ? `${styles.contentWrapper} ${Matchstyles.revealContent}` : `${styles.contentWrapper}` &&  activeMatch !== 3 ? `${styles.hideContentImmediate} ${styles.contentWrapper}` : `${styles.contentWrapper}`}>
                <div className={createFormOpenML ? `${styles.formWrapper}` : `${styles.formWrapper} ${styles.hidden}`}>
                  <div className={styles.formTitle}>
                    Create New Match
                  </div>
                  {this.MatchForm("clusters3")}
                </div>
                <div className={createFormOpenML ? `${styles.formFillerIcon}` : `${styles.formFillerIcon} ${styles.hidden}`}>
                  <CgFormatRight />
                </div>
              </div>
            </div>
            <div 
              className={activeMatch === 3 ? `${styles.table} ${styles.activeTable}` : `${styles.matchTable}`}
            >
              <div className={styles.tableHeader}>
                <div className={Matchstyles.mlColumnOne}>
                  Rank
                </div>
                <div className={Matchstyles.mlColumnTwo}>
                  Match ID
                </div>
                <div className={Matchstyles.mlColumnThree}>
                  Like %
                </div>
                <div className={Matchstyles.mlColumnThree}>
                  Likes
                </div>
                <div className={Matchstyles.mlColumnFive}>
                  Dislikes
                </div>
              </div>
              <Match3 />
            </div>
          </div>
        </div>
        <Modal 
          show={backupFormOpen}
          handler={this.openBackup}
          header="Backup Matches?"
        >
          <div onClick={this.backup} className={Modalstyles.button}>
            CONFIRM BACKUP
          </div>
        </Modal>
        <Modal 
          show={importFormOpen}
          handler={this.openImport}
          header="Import Last Backup?"
        >
          <div onClick={this.import} className={Modalstyles.button}>
            CONFIRM IMPORT
          </div>
        </Modal>
      </Container>
    );
  }
};