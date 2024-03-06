import React, { Component } from "react";
import axios from "axios";
import Container from "../components/Container"
import styles from "../styles/Common.module.css"
import Playerstyles from "../styles/Players.module.css";
import Modalstyles from "../styles/Modal.module.css";

import Modal from "../components/Modal";
import Player from "../components/Player";

import { ImUsers } from "react-icons/im";
import { MdCloudUpload, MdCreate, MdClear, MdHistory } from "react-icons/md";
import { CgFormatRight } from "react-icons/cg";

const api = axios.create({ baseURL: 'http://localhost:8080/'});

export default class PlayersPage extends Component {
  constructor() {
    super();
    this.state = {
      activePlayer: 1,
      prevSearch: 0,
      createFormOpen: false,
      accountID: 0,
      winCnt: 0,
      matchCnt: 0,
      backupFormOpen: false,
      importFormOpen: false
    };
    this.importSuccess = false;
    this.activateRatings = this.activateRatings.bind(this);
    this.activateAnalytics = this.activateAnalytics.bind(this);
    this.toggleCreateForm = this.toggleCreateForm.bind(this);
    this.handleChangeAccountID = this.handleChangeAccountID.bind(this);
    this.handleChangeWinCnt = this.handleChangeWinCnt.bind(this);
    this.handleChangeMatchCnt = this.handleChangeMatchCnt.bind(this);
    this.insertPlayer = this.insertPlayer.bind(this);
    this.import = this.import.bind(this);
  }

  componentDidMount() {
    var searchPl = Number(sessionStorage.getItem('searchPl'));
    searchPl = searchPl ? searchPl : 25;
    this.setState({ prevSearch: searchPl });
    const req = JSON.stringify ({
      "request_type": "search_players",
      "limit": searchPl
    })
    api.post('/search/player', req,
      {headers: {"content-type": "application/json"}})
      .then(res => { console.log(res.data); })
      .catch(err => { console.log(err); })
  }

  componentDidUpdate(){
    sessionStorage.setItem('searchPl', this.state.prevSearch);
  }

  componentWillUnmount(){
    sessionStorage.setItem('searchPl', this.state.prevSearch);
  }

  activateRatings() {
    this.setState({ 
      activePlayer: 1
    })
  }

  activateAnalytics() {
    this.setState({ 
      activePlayer: 2,
      createFormOpen: false,
      accountID: 0,
      winCnt: 0,
      matchCnt: 0
    })
  }

  toggleCreateForm() {
    this.state.createFormOpen ? 
      this.setState({ 
        createFormOpen: false,
        accountID: 0,
        winCnt: 0,
        matchCnt: 0
      })
      : this.setState({ createFormOpen: true });
  }

  handleChangeAccountID(e) {
    this.setState({ accountID: e.target.value });
  }
  handleChangeWinCnt(e) {
    this.setState({ winCnt: e.target.value });
  }
  handleChangeMatchCnt(e) {
    this.setState({ matchCnt: e.target.value });
  }

  insertPlayer(e) {
    e.preventDefault();
    const { accountID, winCnt, matchCnt } = this.state;
    const req = JSON.stringify({
      "request_type": "insert_player",
      "limit": -1,
      "account_id": accountID,
      "win_count": winCnt,
      "match_count": matchCnt
    });
    api.post('/insert/player', req, 
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
    sessionStorage.setItem('searchPl', amount);
    if (this.state.prevSearch !== amount) {
      const req = JSON.stringify ({
        "request_type": "search_players",
        "limit": amount
      })
      api.post('/search/player', req,
        {headers: {"content-type": "application/json"}})
        .then(res => { console.log(res.data); })
        .catch(err => { console.log(err); })
    }
    this.setState({ prevSearch: amount });
  }

  backup() {
    const req = JSON.stringify({
      "request_type": "backup_player"
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
        "request_type": "import_player"
      });
      api.post('/import/common', req, 
        {headers: {"content-type": "application/json"}})
        .then(res => { console.log(res) })
        .catch(err => { console.log(err) })
    }
  }

  render() {
    var searchPl = Number(sessionStorage.getItem('searchPl'));
    searchPl = searchPl ? searchPl : 25;
    
    const { activePlayer, createFormOpen, accountID, winCnt, matchCnt, backupFormOpen, importFormOpen } = this.state;

    return (
      <Container main>
        <div className={styles.container}>
          <div className={styles.wrapper}>
          <div className={styles.choiceWrapper}>
              <h1 className={`${styles.title} ${styles.imageTextWrapper}`}>
                <ImUsers /> 
                <div className={styles.imageText}>Players</div>
              </h1>
              {/* <div className={styles.buttonWrapper}>
                <div 
                  onClick={this.activateRatings} 
                  className={activePlayer === 1 ? `${styles.button} ${styles.activeButton}` : `${styles.button}`}
                >
                  Show Ratings
                </div>
                <div 
                  onClick={this.activateAnalytics} 
                  className={activePlayer === 2 ? `${styles.button} ${styles.activeButton}` : `${styles.button}`}
                >
                  Predict Win Rate
                </div>
              </div> */}
            </div>

            <div className={activePlayer !== 1 ? `${styles.hidden}` : null}>
              <div className={styles.buttonContainer}>
                <div className={styles.buttonWrapper}>
                  <div 
                    onClick={this.toggleCreateForm}
                    className={createFormOpen ? `${styles.button} ${styles.toggleButton}` : `${styles.button}`}
                  >
                    <div className={createFormOpen ? `${styles.buttonIcon} ${styles.hidden}` : `${styles.buttonIcon}`}><MdCreate /></div>
                    <div className={createFormOpen ? `${styles.buttonIcon}` : `${styles.buttonIcon} ${styles.hidden}`}><MdClear /></div>
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
                      className={searchPl === 25 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                    >
                      <div className={styles.buttonNumber}>
                        25
                      </div>
                    </div>
                    <div 
                      onClick={() => this.updateSearchAmount(50)}
                      className={searchPl === 50 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                    >
                      <div className={styles.buttonNumber}>
                        50
                      </div>
                    </div>
                    <div 
                      onClick={() => this.updateSearchAmount(100)}
                      className={searchPl === 100 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                    >
                      <div className={styles.buttonNumber}>
                        100
                      </div>
                    </div>
                  </div>
                </div>
              </div>

              <div className={createFormOpen ? `${styles.formContainer} ${Playerstyles.revealForm}` : `${styles.formContainer}`}>
                <div className={styles.formFiller}></div>
                <div className={createFormOpen ? `${styles.contentWrapper} ${Playerstyles.revealContent}` : `${styles.contentWrapper}`}>
                  <div className={createFormOpen ? `${styles.formWrapper}` : `${styles.formWrapper} ${styles.hidden}`}>
                    <div className={styles.formTitle}>
                      Create New Player
                    </div>
                    <form 
                      className={styles.formContent}
                      onSubmit={this.insertPlayer}
                    >
                      <div className={styles.formSection}>
                        <div className={styles.sectionTitle}>
                          ACCOUNT ID
                        </div>
                        <input required
                          type="text" 
                          pattern="[0-9]+"
                          title="Input only numbers."
                          value={accountID}
                          onChange={this.handleChangeAccountID}
                        />
                      </div>
                      <div className={styles.formSection}>
                        <div className={styles.sectionTitle}>
                          WIN COUNT
                        </div>
                        <input required
                          type="text" 
                          pattern="[0-9]+"
                          title="Input only numbers."
                          value={winCnt}
                          onChange={this.handleChangeWinCnt}
                        />
                      </div>
                      <div className={styles.formSection}>
                        <div className={styles.sectionTitle}>
                          MATCH COUNT
                        </div>
                        <input required
                          type="text" 
                          pattern="[0-9]+"
                          title="Input only numbers."
                          value={matchCnt}
                          onChange={this.handleChangeMatchCnt}
                        />
                      </div>
                      <input type="submit" value="CREATE NEW PLAYER"></input>
                    </form>
                  </div>
                  <div className={createFormOpen ? `${styles.formFillerIcon}` : `${styles.formFillerIcon} ${styles.hidden}`}>
                    <CgFormatRight />
                  </div>
                </div>
              </div>
              <div className={styles.table}>
                <div className={styles.tableHeader}>
                  <div className={Playerstyles.tableColumnOne}>
                    Rank
                  </div>
                  <div className={Playerstyles.tableColumnTwo}>
                    Account ID
                  </div>
                  <div className={Playerstyles.tableColumnThree}>
                    Win %
                  </div>
                  <div className={Playerstyles.tableColumnFour}>
                    Total Wins
                  </div>
                  <div className={Playerstyles.tableColumnFive}>
                    Total Matches
                  </div>
                </div>
                <Player />
              </div>
            </div>
            

            
          </div>
        </div>
        <Modal 
          show={backupFormOpen}
          handler={this.openBackup}
          header="Backup Players?"
        >
          <div onClick={this.backup} className={Modalstyles.button}>
            CONFIRM BACKUP
          </div>
        </Modal>
        <Modal 
          show={importFormOpen}
          handler={this.openImport}
          header="Import Last Session?"
        >
          <div onClick={this.import} className={Modalstyles.button}>
            CONFIRM IMPORT
          </div>
        </Modal>
      </Container>
    );
  }
};