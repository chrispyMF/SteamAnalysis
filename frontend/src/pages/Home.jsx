import { useState } from "react";
import "./Home.css";
import steamLogo from "../assets/steam-logo.png";

export default function Home() {
    // Stores the Steam ID typed by user into input field
    const [steamId, setSteamId] = useState("");

    // Stores the object that will eventually be sent to backend
    // Shows on page now for debugging purposes, REMOVE LATER
    const [payload, setPayload] = useState(null);

    const [error, setError] = useState(null);

    // Checks if the input in field is a valid Steam64 ID or vanity URL, (TODO LATER: add more validation if sending potential invalid input slows backend code)
    const isValidSteamId = (steamId) => {
        const id = steamId.trim();

        // Check if Steam64ID (17 digit int)
        if (/^\d{17}$/.test(id)) return true;

        // Check if vanity URL (3-32 alphanumeric chars)
        //if (/^[a-zA-Z0-9]{3,32}$/.test(id)) return true;

        return false;
    }

    // Runs when form is submitted
    // Prepares the payload object for backend
    const handleSubmit = (e) => {
        // Prevent page from refreshing
        e.preventDefault();

        // Trim whitespace from input
        const trimmedId = steamId.trim();

        // Validate the input before preping payload
        if (!isValidSteamId(trimmedId)) {
            setError("Please enter a valid Steam64 ID.");
            return;
        }

        // No active error
        setError(null);

        // JSON object to send to backend
        // Trims whitespace in case of user error, (TODO: add validation for user input)
        const requestBody = {
            steamId: trimmedId
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

                    {error && (
                        <p style={{ color: "#ff6b6b", fontSize: "0.85rem" }}>{error}</p>
                    )}

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