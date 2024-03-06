import { NavLink } from "react-router-dom";

import Navstyles from "../styles/Nav.module.css";

export default function Nav() {
  return (
    <div className={Navstyles.container}>
      <div className={Navstyles.navwrapper}>
        <ul className={Navstyles.list}>
          <li>
            <NavLink 
              to="/"
              activeClass="active"
              className={`${Navstyles.link} ${Navstyles.home}`}
            >
              TEAM A
            </NavLink>
          </li>
        </ul>
        <ul className={Navstyles.list}>
          <li>
            <NavLink exact
              to="/heroes"
              activeClass="active"
              className={Navstyles.link}
            >
              Heroes
            </NavLink>
          </li>
          <li>
            <NavLink exact
              to="/abilities"
              activeClass="active"
              className={Navstyles.link}
            >
              Abilities
            </NavLink>
          </li>
          <li>
            <NavLink exact
              to="/players"
              activeClass="active"
              className={Navstyles.link}
            >
              Players
            </NavLink>
          </li>
          <li>
            <NavLink exact
              to="/matches"
              activeClassName="active"
              className={Navstyles.link}
            >
              Matches
            </NavLink>
          </li>
        </ul>
      </div>
    </div>
    
  );
}