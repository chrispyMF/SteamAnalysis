import "./Team.css";

// Data for team members, can be expanded later to include more social links
const teamMembers = [
    {
        name: "Chris Materick",
        role: "Frontend Developer",
        github: "https://github.com/chrispyMF",
        githubUsername: "chrispyMF",
        avatar: "https://avatars.githubusercontent.com/u/121414227?v=4"
    },
    {
        name: "Fair Day",
        role: "SUPER AWESOME ROLE",
        github: "https://github.com/Fair-Day-Labs",
        githubUsername: "Fair;Day Labs",
        avatar: "https://avatars.githubusercontent.com/u/212604035?s=200&v=4"
    }
];


export default function Team() {
    return (
        // Full page container
        <div className="team-container">
            {/* Top text and formatting, TODO: Polish this so that it doesn't sound corny asf*/}
            <div className="team-header">
                <h1>Meet The Team</h1>
                <p>
                    Fair;Day Labs is a student founded group focused on building projects using real world approaches the reflect our interests. (idk fix later)
                </p>
            </div>

            {/* Card grid container*/}
            <div className="team-grid">
                {/* Map over team members and create a card for each one*/}
                {teamMembers.map((member) => (
                    <div className="team-card" key={member.name}>

                        {/* GitHub pfp, irl name, role, and github link */}
                        <img src={member.avatar} alt={`${member.name} avatar`} className="team-avatar"/>

                        <h3>{member.name}</h3>

                        <p className="team-role">{member.role}</p>

                        {/* GitHub link with logo, TODO: add more social links later */}
                        <a href={member.github} target="_blank" rel="noreferrer" className="team-github">
                            <img src = "https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png" alt="GitHub Logo" className="team-github-logo"/>
                            <span>{member.githubUsername}</span>
                        </a>

                        <div className="team-spacer"/>

                        <div className="team-socials">{/*Add social buttons later*/}</div>
                        
                    </div>
                ))}
            </div>
        </div> 
    );
}