import React, { Component } from "react";
import axios from "axios";
import Container from "../components/Container";
import styles from "../styles/Common.module.css";
import Herostyles from "../styles/Heroes.module.css";
import Modalstyles from "../styles/Modal.module.css";

import Modal from "../components/Modal";
import Hero from "../components/Hero";

import { GiCrestedHelmet } from "react-icons/gi";
import { MdCloudUpload, MdCreate, MdClear, MdHistory } from "react-icons/md";
import { CgFormatRight } from "react-icons/cg";

const api = axios.create({ baseURL: 'http://localhost:8080/'});

export default class HeroPage extends Component {
  constructor() {
    super();
    this.state = {
      prevSearch: 0,
      createFormOpen: false,
      heroName: "",
      winRate: 0,
      pickRate: 0,
      backupFormOpen: false,
      importFormOpen: false
    };
    this.importSuccess = false;
    this.toggleCreateForm = this.toggleCreateForm.bind(this);
    this.handleChangeHeroName = this.handleChangeHeroName.bind(this);
    this.handleChangeWinRate = this.handleChangeWinRate.bind(this);
    this.handleChangePickRate = this.handleChangePickRate.bind(this);
    this.insertHero = this.insertHero.bind(this);
    this.import = this.import.bind(this);
    this.updateSearchAmount = this.updateSearchAmount.bind(this);
  }

  componentDidMount() {
    var searchHe = Number(sessionStorage.getItem('searchHe'));
    searchHe = searchHe ? searchHe : 25;
    this.setState({ prevSearch: searchHe });
    const req = JSON.stringify ({
      "request_type": "search_heroes",
      "limit": searchHe
    })
    api.post('/search/hero', req,
      {headers: {"content-type": "application/json"}})
      .then(res => { console.log(res.data); })
      .catch(err => { console.log(err); })
  }

  componentDidUpdate(){
    sessionStorage.setItem('searchHe', this.state.prevSearch);
  }

  componentWillUnmount(){
    sessionStorage.setItem('searchHe', this.state.prevSearch);
  }

  toggleCreateForm() {
    this.state.createFormOpen ? 
      this.setState({ 
        createFormOpen: false,
        heroName: "",
        winRate: 0,
        pickRate: 0
      })
      : this.setState({ createFormOpen: true });
  }

  handleChangeHeroName(e) {
    this.setState({ heroName: e.target.value });
  }
  handleChangeWinRate(e) {
    this.setState({ winRate: e.target.value });
  }
  handleChangePickRate(e) {
    this.setState({ pickRate: e.target.value });
  }

  insertHero(e) {
    e.preventDefault();
    const { heroName, winRate, pickRate } = this.state;
    const req = JSON.stringify({
      "request_type": "insert_hero",
      "hero_name": heroName,
      "win_rate": winRate,
      "pick_rate": pickRate
    });
    api.post('/insert/hero', req, 
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
    sessionStorage.setItem('searchHe', amount);
    if (this.state.prevSearch !== amount) {
      const req = JSON.stringify ({
        "request_type": "search_heroes",
        "limit": amount
      })
      api.post('/search/hero', req,
        {headers: {"content-type": "application/json"}})
        .then(res => { console.log(res.data); })
        .catch(err => { console.log(err); })
    }
    this.setState({ prevSearch: amount });
  }

  backup() {
    const req = JSON.stringify({
      "request_type": "backup_hero"
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
        "request_type": "import_hero"
      });
      api.post('/import/common', req, 
        {headers: {"content-type": "application/json"}})
        .then(res => { console.log(res) })
        .catch(err => { console.log(err) })
    }
  }

  render() {
    var searchHe = Number(sessionStorage.getItem('searchHe'));
    searchHe = searchHe ? searchHe : 25;

    const { createFormOpen, heroName, winRate, pickRate, backupFormOpen, importFormOpen } = this.state;

    return (
      <Container main>
        <div className={styles.container}>
          <div className={styles.wrapper}>
            <h1 className={`${styles.title} ${styles.imageTextWrapper}`}>
              <GiCrestedHelmet />
              <div className={styles.imageText}>Heroes</div>
            </h1>
            
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
                    className={searchHe === 25 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      25
                    </div>
                  </div>
                  <div 
                    onClick={() => this.updateSearchAmount(50)}
                    className={searchHe === 50 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      50
                    </div>
                  </div>
                  <div 
                    onClick={() => this.updateSearchAmount(100)}
                    className={searchHe === 100 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      100
                    </div>
                  </div>
                </div>
              </div>
            </div>

            <div className={createFormOpen ? `${styles.formContainer} ${Herostyles.revealForm}` : `${styles.formContainer}`}>
              <div className={styles.formFiller} />
              <div className={createFormOpen ? `${styles.contentWrapper} ${Herostyles.revealContent}` : `${styles.contentWrapper}`}>
                <div className={createFormOpen ? `${styles.formWrapper}` : `${styles.formWrapper} ${styles.hidden}`}>
                  <div className={styles.formTitle}>
                    Create New Hero
                  </div>
                  <form 
                    className={styles.formContent}
                    onSubmit={this.insertHero}
                  >
                    <div className={styles.formSection}>
                      <div className={styles.sectionTitle}>
                        HERO NAME
                      </div>
                      <input required
                        type="text" 
                        pattern="[a-zA-Z]+||[a-zA-Z]+[- ][a-zA-Z]+"
                        title="Input only letters (dashes and spaces are valid in between letters)."
                        value={heroName}
                        onChange={this.handleChangeHeroName}
                      />
                    </div>
                    <div className={styles.formSection}>
                      <div className={styles.sectionTitle}>
                        WIN RATE (%)
                      </div>
                      <input required
                        type="text" 
                        pattern="[0-9]+.[0-9]+"
                        title="Input only numbers as a percentage."
                        value={winRate}
                        onChange={this.handleChangeWinRate}
                      />
                    </div>
                    <div className={styles.formSection}>
                      <div className={styles.sectionTitle}>
                        PICK RATE (%)
                      </div>
                      <input required
                        type="text" 
                        pattern="0.[0-9]+"
                        title="Input only numbers as a percentage."
                        value={pickRate}
                        onChange={this.handleChangePickRate}
                      />
                    </div>
                    <input type="submit" value="CREATE NEW HERO"></input>
                  </form>
                </div>
                <div className={createFormOpen ? `${styles.formFillerIcon}` : `${styles.formFillerIcon} ${styles.hidden}`}>
                  <CgFormatRight />
                </div>
              </div>
            </div>
            <div className={styles.table}>
              <div className={styles.tableHeader}>
                <div className={Herostyles.tableColumnOne}>
                  Rank
                </div>
                <div className={Herostyles.tableColumnTwo}>
                  Hero
                </div>
                <div className={Herostyles.tableColumnThree}>
                  Win %
                </div>
                <div className={Herostyles.tableColumnFour}>
                  Pick %
                </div>
              </div>
              <Hero />
            </div>
          </div>
        </div>
        <Modal 
          show={backupFormOpen}
          handler={this.openBackup}
          header="Backup Heroes?"
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
}