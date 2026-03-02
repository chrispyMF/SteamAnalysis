import { Link } from "react-router-dom";
import "./Navbar.css";

export default function Navbar() {
    return (
        <nav className="navbar">
            <div className="navbar-left">
                <Link to="/">Steam Analysis</Link>
            </div>

            <div className="navbar-right">
                <Link to="/">Home</Link>
                <Link to="/about">About</Link>
                <Link to="/team">Team</Link>
            </div>
        </nav>
    )
}