import "./Team.css";

const teamMembers = [
    {
        name: "Chris Materick",
        role: "Frontend Developer",
        github: "https://github.com/chrispyMF",
        avatar: "https://avatars.githubusercontent.com/u/121414227?v=4"
    },
    {
        name: "Fair Day",
        role: "SUPER AWESOME ROLE",
        github: "https://github.com/chrispyMF",
        avatar: "https://avatars.githubusercontent.com/u/212604035?s=200&v=4"
    }
];

export default function Team() {
    return (
        <div className="team-container">
            <div className="team-header">
                <h1>Meet The Team</h1>
                <p>
                    ADD TEAM DESCRIPTION HERE!
                </p>
            </div>

            <div className="team-grid">
                {teamMembers.map((member) => (
                    <div className="team-card" key={member.name}>

                        <img src={member.avatar} alt={`${member.name} avatar`} className="team-avatar"/>

                        <h3>{member.name}</h3>

                        <p className="team-role">{member.role}</p>

                        <a href={member.github} target="_blank" rel="noreferrer" className="team-github">GitHub Profile</a>
                    </div>
                ))}
            </div>
        </div> 
    );
}