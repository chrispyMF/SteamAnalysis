import { useState } from "react";
import "./Home.css";
import steamLogo from "../assets/steam-logo.png";

export default function Home() {
    // Holds user input
    const [steamId, setSteamId] = useState("");

    // Holds payload for backend
    const [payload, setPayload] = useState(null);

    // Form submission, prepares payload
    const handleSubmit = (e) => {
        e.preventDefault();

        const requestBody = {
            steamId: steamId.trim()
        };

        setPayload(requestBody);

        console.log("Prepared payload:",  requestBody);
    };

    return (
        <div className="home-container">
            <div className="home-card">
                <img src={steamLogo} alt= "Steam Logo" className="home-logo"/>

                <h1 className="home-title">Steam Analysis</h1>

                <form className="home-form" onSubmit={handleSubmit}>
                    <label>Enter a Steam ID</label>
                    <input
                        type="text"
                        value={steamId}
                        onChange={(e) => setSteamId(e.target.value)}
                        placeholder="7656119XXXXXXXXXX"
                        style={{ display: "block", width: "100%", marginTop: "8px"}}
                    />

                    <button type="submit" style={{ marginTop: 16 }}>
                        Get Recommendations
                    </button>
                </form>

                {payload && (
                    <pre style={{ marginTop: 20 }}>
                        {JSON.stringify(payload, null, 2)}
                    </pre>
                )}
            </div>
        </div>
    );
}