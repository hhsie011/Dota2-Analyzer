import React, { Component } from "react";
import axios from "axios";
import Container from "../components/Container";
import styles from "../styles/Common.module.css";
import Abilitystyles from "../styles/Abilities.module.css";
import Modalstyles from "../styles/Modal.module.css";

import Modal from "../components/Modal";
import Ability from "../components/Ability";

import { GiIciclesFence } from "react-icons/gi";
import { MdCloudUpload, MdCreate, MdClear, MdHistory } from "react-icons/md";
import { CgFormatRight } from "react-icons/cg";

const api = axios.create({ baseURL: 'http://localhost:8080/'});

export default class HeroPage extends Component {
  constructor() {
    super();
    this.state = {
      prevSearch: 0,
      createFormOpen: false,
      abilityName: "",
      upgradeCnt: 0,
      backupFormOpen: false,
      importFormOpen: false
    };
    this.importSuccess = false;
    this.toggleCreateForm = this.toggleCreateForm.bind(this);
    this.handleChangeAbilityName = this.handleChangeAbilityName.bind(this);
    this.handleChangeUpgradeCnt = this.handleChangeUpgradeCnt.bind(this);
    this.insertAbility = this.insertAbility.bind(this);
    this.import = this.import.bind(this);
    this.updateSearchAmount = this.updateSearchAmount.bind(this);
  }

  componentDidMount() {
    var searchAb = Number(sessionStorage.getItem('searchAb'));
    searchAb = searchAb ? searchAb : 25;
    this.setState({ prevSearch: searchAb });
    const req = JSON.stringify ({
      "request_type": "search_top_abilities",
      "limit": searchAb
    })
    api.post('/search/ability', req,
      {headers: {"content-type": "application/json"}})
      .then(res => { console.log(res.data); })
      .catch(err => { console.log(err); })
  }

  componentDidUpdate(){
    sessionStorage.setItem('searchAb', this.state.prevSearch);
  }

  componentWillUnmount(){
    sessionStorage.setItem('searchAb', this.state.prevSearch);
  }

  toggleCreateForm() {
    this.state.createFormOpen ? 
      this.setState({ 
        createFormOpen: false,
        abilityName: "",
        upgradeCnt: 0
      })
      : this.setState({ createFormOpen: true });
  }

  handleChangeAbilityName(e) {
    this.setState({ abilityName: e.target.value });
  }
  handleChangeUpgradeCnt(e) {
    this.setState({ upgradeCnt: e.target.value });
  }

  insertAbility(e) {
    e.preventDefault();
    const { abilityName, upgradeCnt } = this.state;
    const req = JSON.stringify({
      "request_type": "insert_ability",
      "limit": -1,
      "ability_name": abilityName,
      "upgrade_count": upgradeCnt,
    });
    api.post('/insert/ability', req, 
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
    sessionStorage.setItem('searchAb', amount);
    if (this.state.prevSearch !== amount) {
      const req = JSON.stringify ({
        "request_type": "search_top_abilities",
        "limit": amount
      })
      api.post('/search/ability', req,
        {headers: {"content-type": "application/json"}})
        .then(res => { console.log(res.data); })
        .catch(err => { console.log(err); })
    }
    this.setState({ prevSearch: amount });
  }

  backup() {
    const req = JSON.stringify({
      "request_type": "backup_ability"
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
        "request_type": "import_ability"
      });
      api.post('/import/common', req, 
        {headers: {"content-type": "application/json"}})
        .then(res => { console.log(res) })
        .catch(err => { console.log(err) })
    }
  }

  render() {
    var searchAb = Number(sessionStorage.getItem('searchAb'));
    searchAb = searchAb ? searchAb : 25;

    const { createFormOpen, abilityName, upgradeCnt, backupFormOpen, importFormOpen } = this.state;
    
    return (
      <Container main>
        <div className={styles.container}>
          <div className={styles.wrapper}>
            <h1 className={`${styles.title} ${styles.imageTextWrapper}`}>
              <GiIciclesFence />
              <div className={styles.imageText}>Top Abilities</div>
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
                    className={searchAb === 25 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      25
                    </div>
                  </div>
                  <div 
                    onClick={() => this.updateSearchAmount(50)}
                    className={searchAb === 50 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      50
                    </div>
                  </div>
                  <div 
                    onClick={() => this.updateSearchAmount(100)}
                    className={searchAb === 100 ? `${styles.button} ${styles.buttonAlt} ${styles.activeButton}` : `${styles.button} ${styles.buttonAlt}`}
                  >
                    <div className={styles.buttonNumber}>
                      100
                    </div>
                  </div>
                </div>
              </div>
            </div>

            <div className={createFormOpen ? `${styles.formContainer} ${Abilitystyles.revealForm}` : `${styles.formContainer}`}>
              <div className={styles.formFiller}></div>
              <div className={createFormOpen ? `${styles.contentWrapper} ${Abilitystyles.revealContent}` : `${styles.contentWrapper}`}>
                <div className={createFormOpen ? `${styles.formWrapper}` : `${styles.formWrapper} ${styles.hidden}`}>
                  <div className={styles.formTitle}>
                    Create New Ability
                  </div>
                  <form 
                    className={styles.formContent}
                    onSubmit={this.insertAbility}
                  >
                    <div className={styles.formSection}>
                      <div className={styles.sectionTitle}>
                        ABILITY NAME
                      </div>
                      <input required
                        type="text" 
                        pattern="[a-zA-Z]+||[a-zA-Z]+_[a-zA-Z]+"
                        title="Input only letters (underscores are valid in between letters)."
                        value={abilityName}
                        onChange={this.handleChangeAbilityName}
                      />
                    </div>
                    <div className={styles.formSection}>
                      <div className={styles.sectionTitle}>
                        UPGRADE COUNT
                      </div>
                      <input required
                        type="text" 
                        pattern="[0-9]+"
                        title="Input only numbers."
                        value={upgradeCnt}
                        onChange={this.handleChangeUpgradeCnt}
                      />
                    </div>
                    <input type="submit" value="CREATE NEW ABILITY"></input>
                  </form>
                </div>
                <div className={createFormOpen ? `${styles.formFillerIcon}` : `${styles.formFillerIcon} ${styles.hidden}`}>
                  <CgFormatRight />
                </div>
              </div>
            </div>
            <div className={styles.table}>
              <div className={styles.tableHeader}>
                <div className={Abilitystyles.tableColumnOne}>
                  Rank
                </div>
                <div className={Abilitystyles.tableColumnTwo}>
                  Ability
                </div>
                <div className={Abilitystyles.tableColumnThree}>
                  Total Times Upgraded
                </div>
              </div>
              <Ability />
            </div>
          </div>
        </div>
        <Modal 
          show={backupFormOpen}
          handler={this.openBackup}
          header="Backup Abilities?"
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