import { useState } from "react";
import "./Home.css";
import steamLogo from "../assets/steam-logo.png";

export default function Home() {
    // Stores the Steam ID typed by user into input field
    const [steamId, setSteamId] = useState("");

    // Stores the object that will eventually be sent to backend
    // Shows on page now for debugging purposes, REMOVE LATER
    const [payload, setPayload] = useState(null);

    // Runs when form is submitted
    // Prepares the payload object for backend
    const handleSubmit = (e) => {
        // Prevent page from refreshing
        e.preventDefault();

        // JSON object to send to backend
        // Trims whitespace in case of user error, (TODO: add validation for user input)
        const requestBody = {
            steamId: steamId.trim()
        };

        // Save the payload to state so it can be displayed on page for debugging purposes, REMOVE LATER
        setPayload(requestBody);

        console.log("Prepared payload:",  requestBody);
    };

    return (
        // Full-page container (background + centering)
        <div className="home-container">
            {/* Card with logo, title, and form */}
            <div className="home-card">

                {/* Steam logo at top of card, (TODO: figure out best place for logo, also get a new logo) */}
                <img src={steamLogo} alt= "Steam Logo" className="home-logo"/>

                {/* Title of the page, (TODO: come up with better title) */}
                <h1 className="home-title">Steam Analysis</h1>

                {/* Form for user input, (TODO: add validation before sending to backend) */}
                <form className="home-form" onSubmit={handleSubmit}>
                    <label>Enter a Steam ID</label>
                    <input
                        type="text"
                        value={steamId}
                        onChange={(e) => setSteamId(e.target.value)}
                        placeholder="7656119XXXXXXXXXX"
                        style={{ display: "block", width: "100%", marginTop: "8px"}}
                    />
                    {/* Submit button (TODO: Figure out what we want it to say) */}
                    <button type="submit" style={{ marginTop: 16 }}>
                        Get Recommendations
                    </button>
                </form>

                {/* Show prepared payload for debugging, REMOVE LATER */}
                {payload && (
                    <pre style={{ marginTop: 20 }}>
                        {JSON.stringify(payload, null, 2)}
                    </pre>
                )}
            </div>
        </div>
    );
}