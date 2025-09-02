DROP TABLE IF EXISTS artiste CASCADE;
DROP TABLE IF EXISTS groupe CASCADE;
DROP TABLE IF EXISTS appartient CASCADE;
DROP TABLE IF EXISTS album CASCADE;
DROP TABLE IF EXISTS musique CASCADE;
DROP TABLE IF EXISTS cree CASCADE;
DROP TABLE IF EXISTS playlist CASCADE;
DROP TABLE IF EXISTS contient CASCADE;
DROP TABLE IF EXISTS ecoute CASCADE;
DROP TABLE IF EXISTS ecoutePlay CASCADE;
DROP TABLE IF EXISTS suit CASCADE;
DROP TABLE IF EXISTS suitGroupe CASCADE;
DROP TABLE IF EXISTS abonnement CASCADE;
DROP TABLE IF EXISTS utilisateur CASCADE;

CREATE TABLE artiste(
    id serial PRIMARY KEY,
    nom varchar(30) NOT NULL,
    prenom varchar(30) NOT NULL,        
    nationalite varchar(25),
    naissance date NOT NULL,
    mort date 
);

CREATE TABLE groupe(
    idG int PRIMARY KEY,
    nom varchar(50) NOT NULL,
    nationalite varchar(50) NOT NULL,
    genre varchar(50),
    dateG date
);

CREATE TABLE appartient(
    id int REFERENCES artiste(id) ON DELETE CASCADE,
    idG int REFERENCES groupe(idG) ON DELETE CASCADE,
    role varchar(50) NOT NULL,
    arriver date NOT NULL,
    depart date,
    PRIMARY KEY(id, idG),
    CONSTRAINT chk_dates_depart CHECK (
        (depart IS NULL) OR (depart >= arriver)
    )
);

CREATE TABLE album(
    idA serial PRIMARY KEY,
    titre varchar(50) NOT NULL,
    dateParution date NOT NULL,
    image_path VARCHAR(255),  
    description text,
    idG int REFERENCES groupe(idG) ON DELETE CASCADE
);

CREATE TABLE musique(
    idM int PRIMARY KEY,
    titre varchar(50) NOT NULL,
    duree time NOT NULL,
    idA int REFERENCES album(idA) ON DELETE CASCADE,
    parole text
);

CREATE TABLE cree(
    idM int REFERENCES musique(idM) ON DELETE CASCADE,
    id int REFERENCES artiste(id) ON DELETE CASCADE,
    PRIMARY KEY(idM, id)
);

CREATE TABLE utilisateur(
    idU serial PRIMARY KEY,
    pseudo varchar(50) NOT NULL UNIQUE,
    mail varchar(40) NOT NULL,
    motdepasse varchar(50) UNIQUE,
    dateArriver date NOT NULL
);



CREATE TABLE playlist(
    idP serial PRIMARY KEY,
    titre varchar(50),
    description text UNIQUE,
    prive BOOLEAN NOT NULL,
    idU int REFERENCES utilisateur(idU) ON DELETE CASCADE
);

CREATE TABLE contient(
    idM int REFERENCES musique(idM),
    idP int REFERENCES playlist(idP),
    PRIMARY KEY(idM, idP)
);

CREATE TABLE suit(
    idU int REFERENCES utilisateur(idU) ON DELETE CASCADE,
    id int REFERENCES artiste(id) ON DELETE CASCADE,
    PRIMARY KEY(idU, id)
);

CREATE TABLE abonnement (
    idU int REFERENCES utilisateur(idU) ON DELETE CASCADE, 
    idAbonne int REFERENCES utilisateur(idU) ON DELETE CASCADE, 
    PRIMARY KEY (idU, idAbonne),
    CONSTRAINT chk CHECK (idU != idAbonne)
);

CREATE TABLE suitGroupe (
    idU int REFERENCES utilisateur(idU) ON DELETE CASCADE, 
    idG int REFERENCES groupe(idG) ON DELETE CASCADE, 
    PRIMARY KEY (idU, idG)
);

CREATE TABLE ecoute(
    idE serial PRIMARY KEY,
    idU int REFERENCES utilisateur(idU) ON DELETE CASCADE,
    idA int REFERENCES album(idA) ON DELETE CASCADE,
    idM int REFERENCES musique(idM) ON DELETE CASCADE
);

CREATE TABLE ecoutePlay (
    idEP serial PRIMARY KEY,
    idU int REFERENCES utilisateur(idU) ON DELETE CASCADE,
    idP int REFERENCES playlist(idP) ON DELETE CASCADE,
    idM int REFERENCES musique(idM) ON DELETE CASCADE
);

CREATE VIEW nbecouteUnique AS (
    SELECT idM, count(idu) AS nbE
    FROM ecoute 
    GROUP BY idM
);

CREATE VIEW nbpersonne AS (
    SELECT idM, count(distinct idu) AS nbP
    FROM ecoute 
    GROUP BY idM
);

CREATE VIEW nbPartageParMorceau AS(
    SELECT ecoutePlay.idM, COUNT(DISTINCT ecoutePlay.idU) AS nbPartage
    FROM ecoutePlay
    JOIN playlist ON ecoutePlay.idP = playlist.idP
    WHERE playlist.prive = false
    GROUP BY ecoutePlay.idM
);

CREATE VIEW remu AS (
    SELECT idM, (count(distinct idu) * 0.10 + (count(idu) - count(distinct idu)) * 0.01) AS remuneration
    FROM ecoute 
    GROUP BY idM
);

insert into artiste VALUES 
    (1, 'Keys', 'Alicia', 'Americaine', '1981-01-25', NULL),
    (2, 'Gene Hernandez', 'Peter', 'Hawaien', '1985-10-08', NULL),
    (3, 'Buble', 'Micheal', 'Canadien', '1975-09-09',NULL),
    (4, 'Joseph Jackson', 'Michael', 'Americain', '1958-08-29', '2009-06-25'),
    (5, 'Carey', 'Mariah', 'Americaine', '1969-03-27', NULL),
    (6,'Laurie Blue adkins', 'Adele', 'Anglaise', '1988-05-05', NULL);


insert into groupe VALUES 
    (1,'Bruno Mars','Hawaien', 'Pop', '2004-01-01'),
    (2, 'Alicia Keys', 'Americaine', 'R&B', '2001-04-05'),
    (3, 'Micheal Bublé', 'Canadien', 'jazz-pop', '1995-01-01'),
    (4, 'Michael Jackson', 'Americain', 'R&B-disco-pop', '1984-01-01');



insert into appartient VALUES
    (1,2, 'chanteur','2001-04-05', NULL),
    (2,1, 'chanteur', '2004-01-01', NULL),
    (3,3, 'chanteur', '1995-01-01', NULL),
    (4,4, 'chanteur', '1984-01-01', '2009-06-25');



insert into album VALUES
    (1, 'Christmas', '2011-10-21', 'christmas.jpg', 'L album le plus connu de Micheal Bublé et l un des meilleurs album du 21ème siècle',3),
    (2, 'Off The Walls', '1979-08-10', 'offthewall.jpg', 'Sorti trois semaines avant le 21e anniversaire de Michael Jackson, Off the Wall marque sa première collaboration avec le producteur Quincy Jones. Les thèmes incluent notamment l amour, la fête, l évasion, la liberté ou encore la solitude.', 4),
    (3, 'Songs In A Minor', '2001-04-05', 'alicia.jpg', 'Premier album d Alicia Keys qui lui a fait remporter 5 Grammy Awards en 2002.',2),
    (4, 'Thriller', '1982-11-30', 'thriller.jpeg', 'Premier album à demeurer 37 semaines au sommet du Billboard 200, Thriller devient l album le plus vendu au monde avec 32 millions d exemplaires vendus en une seule année.',4),
    (5, 'Unothodox Jkebox', '2012-12-07','uno.jpeg', 'Deuxième album du chanteur Bruno Mars qui à atteint les 5 millions de copies vendu fin 2013',1);



insert into musique VALUES 
    (1, 'A Woman s Worth', '00:05:03', 3,
'You could buy me diamonds, you could buy me pearls
Take me on a cruise around the world
(Baby, you know I m worth it)
Dinner lit by candles, run my bubble bath
Make love tenderly to last and last
(Baby, you know I m worth it)

Wanna please, wanna keep, wanna treat your woman right
Not just dough, better show that you know she s worth your time (that s right, that s right)
You will lose if you choose to refuse to put her first (that s right)
She will and she can find a man who knows her worth

(Cause a, cause a, cause a)
Cause a real man knows a real woman when he sees her
And a real woman knows a real man ain t fraid to please her
And a real woman knows a real man always comes first
And a real man just can t deny a woman s worth (break it down)

Oh-oh, oh-oh (na-na-na-na-na-na)
Oh-oh, oh-oh (mm-hm)
Oh-oh, oh-oh
Oh (let s do it like this, come on)

If you treat me fairly (fairly)
I ll give you all my goods (I ll give you all my goods)
Treat you like a real woman should
(Baby, I know you re worth it)
If you never play me (play me)
Now don t you bluff (don t you bluff)
I ll hold you down when shit gets rough
(Cause, baby, I know you re worth it)

She walks the mile, makes you smile, all the while being true (yeah, yeah)
Don t take for granted the passion that she has for you (what s that, what s that?)
You will lose if you choose to refuse to put her first (you better recognize it)
She will and she can find a man who knows her worth, ooh-whoa

Cause a real man knows a real woman when he sees her
And a real woman knows a real man ain t fraid to please her
And a real woman knows a real man always comes first
And a real man just can t deny a woman s worth

Yeah, someone who know
Do I have some, do I have some real man
Out in there, out in the audience tonight?
Hey, don t be ashamed, you know what I mean
You gotta give it up, if you are real then you re real
Someone who know, do I have some real woman in this audience tonight?
Hey, yeah, now I have another question for you
I wanna know, you all think that you wanna hear something new
(Yeah)
Lets flip it around on the horns'),
 (2, 'Billie Jean', '00:04:55', 4,
'She was more like a beauty queen from a movie scene
I said, Don t mind, but what do you mean, I am the one
Who will dance on the floor in the round?
She said, I am the one
Who will dance on the floor in the round

She told me her name was Billie Jean, as she caused a scene
Then every head turned with eyes that dreamed of being the one
Who will dance on the floor in the round

People always told me, Be careful of what you do
And don t go around breaking young girls hearts
And mother always told me, Be careful of who you love
And be careful of what you do
 Cause the lie becomes the truth, hey-ey

Billie Jean is not my lover
She s just a girl who claims that I am the one
But the kid is not my son
She says I am the one
But the kid is not my son (oh no)

For 40 days and 40 nights
The law was on her side
But who can stand when she s in demand
Her schemes and plans
Cause we danced on the floor in the round
So take my strong advice
Just remember to always think twice
(Do think twice, do think twice)

She told my baby, we d danced til three
Then she looked at me
Then showed a photo of a baby prize
His eyes were like mine (oh no)
Cause we danced on the floor in the round, baby

People always told me, Be careful of what you do
And don t go around breaking young girls hearts (don t break no hearts)
But she came and stood right by me
Just the smell of sweet perfume
This happened much too soon
She called me to her room, hey-ey

Billie Jean is not my lover
She s just a girl who claims that I am the one
But the kid is not my son

Billie Jean is not my lover
She s just a girl who claims that I am the one
But the kid is not my son (no, no)
She says I am the one (oh yeah)
But the kid is not my son (hee-hee-hee)

Hee!
Whoo!

She says I am the one
But the kid is not my son

The Billie Jean is not my lover
She s just a girl who claims that I am the one
(You know what you did to me, baby)
But the kid is not my son (no, no, no)
She says I am the one (no, no)
But the kid is not my son (no, no, no)

She says I am the one (you know what you did)
She says he is my son (breaking my heart, babe)
She says I am the one

Billie Jean is not my lover
The Billie Jean is not my lover
The Billie Jean is not my lover
Billie Jean is not my lover (don t Billie Jean)
Billie Jean is not my lover (she s not what she seems)
Billie Jean is-
Billie Jean'),
 (3, 'Don t Stop Til You Get Enough','00:06:04', 2,
'You know, I was
I was wondering, you know, if
If you could keep on, because
The force, it
It s got a lot of power, and
It make me feel like, uh
It, it make me feel like, uh (whoo!)

Lovely is the feeling now
Fever, temperature s rising now
Power (oh, power) is the force, the vow
That makes it happen, it asks no questions why (ooh)
So get closer (closer now) to my body now
Just love me  til you don t know how (ooh)

Keep on with the force, don t stop
Don t stop til you get enough
Keep on with the force, don t stop
Don t stop til you get enough
Keep on with the force, don t stop
Don t stop til you get enough
Keep on with the force, don t stop
Don t stop  til you get enough

Touch me, and I feel on fire
Ain t nothing like a love desire (ooh)
I m melting (I m melting now) like hot candle wax
Sensation (ah, sensation)
Lovely where we re at (ooh)
So let love take us through the hours
I won t be complaining (ooh)
Oh,  cause this is love power (ooh)

Keep on with the force, don t stop
Don t stop  til you get enough
Keep on with the force, don t stop
Don t stop til you get enough
Keep on with the force, don t stop
Don t stop  til you get enough
Keep on with the force, don t stop
Don t stop til you get enough

Ooh
Oh, baby
Keep on, keep on

Heartbreak, enemy despise
Eternal (oh, eternal) love shines in my eyes (ooh)
So let love (oh, let love) take us through the hours
I won t be complaining (no, no), ooh
Cause your love is alright, alright (ooh)

Keep on with the force, don t stop
Don t stop til you get enough
Keep on with the force, don t stop
Don t stop til you get enough
Keep on with the force, don t stop
Don t stop til you get enough
Keep on with the force, don t stop
Don t stop til you get enough

Keep on with the force, don t stop (don t stop, baby)
Don t stop til you get enough
Keep on with the force, don t stop
Don t stop til you get enough (oh, my baby)
Keep on with the force, don t stop
Don t stop til you get enough
Keep on with the force, don t stop
Don t stop til you get enough

Lovely is the feeling now
I won t be complaining (ooh, ooh)
The force is love power

Keep on with the force, don t stop
Don t stop til you get enough
Keep on with the force, don t stop (don t stop, darling)
Don t stop til you get enough
Keep on with the force, don t stop (no, baby)
Don t stop til you get enough
Keep on with the force, don t stop (keep on now)
Don t stop til you get enough

Keep on with the force, don t stop (keep on, darling)
Don t stop til you get enough
Keep on with the force, don t stop (no, baby, yeah)
Don t stop  til you get enough
Keep on with the force, don t stop (keep on, darling)
Don t stop til you get enough
Keep on with the force, don t stop (no, no)
Don t stop til you get enough

Keep on with the force, don t stop (keep on with the power, baby, ooh)
Don t stop til you get enough
Keep on with the force, don t stop (til you get enough)
Don t stop til you get enough (ooh)
Keep on with the force, don t stop (yeah)
Don t stop  til you get enough (oh, no)
Keep on with the force, don t stop (yeah)
Don t stop til you get enough

Keep on with the force, don t stop (keep on, baby)
Don t stop til you get enough
Keep on with the force, don t stop (til you get enough)
Don t stop til you get enough
Keep on with the force, don t stop (keep on, darling)
Don t stop til you get enough
Keep on with the force, don t stop (oh, no)
Don t stop til you get enough (keep on)

Keep on with the force, don t stop
Don t stop til you get enough
Keep on with the force, don t stop'),
    (4, 'Fallin', '00:03:29', 3,
'I keep on fallin in
And out of love with you
Sometimes I love you
Sometimes you make me blue

Sometimes I feel good
At times I feel used
Lovin you darlin
Makes me so confused

I keep on fallin in and out
Of love with you
I never loved someone
The way that I love you

Oh, oh, I

Never felt this way
How do you give me so much pleasure
And cause me so much pain? (Yeah, yeah)
Just when I think
I ve taken more than would a fool
I start fallin back in love with you

I keep on fallin in and out
Of love with you
I never loved someone
The way that I love you

Oh, baby
I, I, I, I m fallin (Yeah, yeah)
I, I, I, I m fallin
Fall, fall, fall (Sing)
Fall

I keep on fallin in and out (Out)
Of love (Love), with you (With you)
I never loved someone
The way that I love you

I m fallin in and out
Of love (Of love), with you (With you)
I never loved someone
The way that I love you

I m fallin in and out
Of love (Of love), with you (With you)
I never (No), loved someone (No, no)
The way that I love you
What?'),
 (5, 'It s Beginning to Look a Lot Like Christmas', '00:03:27',1,
'It s beginning to look a lot like Christmas
Everywhere you go
Take a look at the five and ten
It s glistening once again
With candy canes and silver lanes aglow

It s beginning to look a lot like Christmas
Toys in every store
But the prettiest sight to see is the holly that will be
On your own front door

A pair of hop-a-long boots and a pistol that shoots
Is the wish of Barney and Ben
Dolls that ll talk and will go for a walk
Is the hope of Janice and Jen
And Mom and Dad can hardly wait for school to start again

It s beginning to look a lot like Christmas
Everywhere you go
There s a tree in the grand hotel, one in the park as well
It s the sturdy kind that doesn t mind the snow

It s beginning to look a lot like Christmas
Soon the bells will start
And the thing that will make them ring is the carol that you sing
Right within your heart

It s beginning to look a lot like Christmas
Toys in every store
But the prettiest sight to see is the holly that will be
On your own front door

Sure it s Christmas once more'),
    (6, 'Locked Out of Heaven', '00:03:53', 5, 
'Oh, yeah, yeah
Oh, yeah, yeah, yeah, yeah (ooh)
Oh, yeah, yeah
Oh, yeah, yeah, yeah, yeah (ooh)

Never had much faith in love or miracles (ooh)
Never wanna put my heart on the line (ooh)
But swimmin  in your water s something spiritual (ooh)
I m born again every time you spend the night-ight (ooh)

Cause your sex takes me to paradise
Yeah, your sex takes me to paradise
And it shows
Yeah, yeah, yeah

Cause you make me feel like
I ve been locked out of heaven
For too long, for too long
Yeah, you make me feel like
I ve been locked out of heaven
For too long, for too long, oh-oh-oh-oh

Oh, yeah, yeah, yeah, yeah (ooh)
Oh, yeah, yeah
Oh, yeah, yeah, yeah, yeah (ooh)

You bring me to my knees, you make me testify (ooh)
You can make a sinner change his ways (ooh)
Open up your gates, cause I can t wait to see the light (ooh)
And right there is where I wanna stay-ay (ooh)

Cause your sex takes me to paradise
Yeah, your sex takes me to paradise
And it shows
Yeah, yeah, yeah

Cause you make me feel like
I ve been locked out of heaven
For too long, for too long
Yeah, you make me feel like
I ve been locked out of heaven
For too long, for too long, oh

Oh, whoa, whoa, whoa, yeah, yeah, yeah
Can I just stay here?
Spend the rest of my days here?
Oh, whoa, whoa, whoa, yeah, yeah, yeah
Can I just stay here?
Spend the rest of my days here?

Cause you make me feel like
I ve been locked out of heaven
For too long, for too long
Yeah, you make me feel like
I ve been locked out of heaven
For too long, for too long, oh-oh-oh-oh

Oh, yeah, yeah, yeah, yeah (ooh)
Oh, yeah, yeah
Oh, yeah, yeah, yeah, yeah (ooh)' ),
 (7, 'Thriller', '00:05:57', 4,
'It s close to midnight
Something evil s lurkin in the dark
Under the moonlight
You see a sight that almost stops your heart
You try to scream
But terror takes the sound before you make it
You start to freeze
As horror looks you right between the eyes
You re paralyzed

Cause this is thriller
Thriller night
And no one s gonna save you
From the beast about to strike

You know it s thriller
Thriller night
You re fighting for your life
Inside a killer, thriller tonight, yeah

You hear the door slam
And realize there s nowhere left to run
You feel the cold hand
And wonder if you ll ever see the sun

You close your eyes
And hope that this is just imagination, girl
But all the while
You hear a creature creepin up behind
You re out of time

Cause this is thriller
Thriller night
There ain t no second chance
Against the thing with the forty eyes, girl

(Thriller, thriller night)
You re fighting for your life
Inside a killer, thriller tonight

Night creatures callin
The dead start to walk in their masquerade
There s no escaping the jaws of the alien this time
(They re open wide)
This is the end of your life

They re out to get you
There s demons closing in on every side
They will possess you
Unless you change that number on your dial

Now is the time
For you and I to cuddle close together, yeah
All through the night
I ll save you from the terror on the screen
I ll make you see

That this is thriller
Thriller night
Cause I can thrill you more
Than any ghoul would ever dare try

(Thriller, thriller night)
So let me hold you tight
And share a
(Killer, diller, chiller)
(Thriller here tonight)

Cause this is thriller
Thriller night
Girl, I can thrill you more
Than any ghoul would ever dare try

(Thriller, thriller night)
So let me hold you tight
And share a
(Killer, thriller)

I m gonna thrill you tonight

Darkness falls across the land
The midnight hour is close at hand
Creatures crawl in search of blood
To terrorize y alls neighborhood
And whosoever shall be found
Without the soul for getting down
Must stand and face the hounds of hell
And rot inside a corpse s shell

I m gonna thrill you tonight (Thriller)
Ooh babe
(Thriller) I m gonna thrill you tonight
(Thriller) Oh darling
(Oh babe) I m gonna thrill you tonight
(Thriller) Ooh, babe
(Thriller at night, babe) I m gonna thrill you tonight
(Thriller) Oh darling
(Oh babe) Thriller at night, babe
(Thriller at night, babe)

The foulest stench is in the air
The funk of forty thousand years
And grisly ghouls from every tomb
Are closing in to seal your doom
And though you fight to stay alive
Your body starts to shiver
For no mere mortal can resist
The evil of the thriller

Ah ah ah ah ah ah ah ah ah ah ah
Ah ah ah ah ah ah ah ah ah ah ah ah ah ah ah ah ah ah
Ah ah ah ah ah ah ah ah ah ah ah
'),
    (8, 'Treasure', '00:02:56', 5, 
'Gimme your, gimme your, gimme your attention, baby
I gotta tell you a little somethin about yourself
You re wonderful, flawless, ooh, you a sexy lady
But you walk around here like you wanna be someone else

Oh, whoa
I know that you don t know it, but you re fine, so fine
(Fine, so fine)
Oh, whoa
Oh girl, I m gonna show you when you re mine, oh, mine
(Mine, oh, mine)

Treasure, that is what you are
Honey, you re my golden star
You know you can make my wish come true
If you let me treasure you
If you let me treasure you

Pretty girl, pretty girl, pretty girl, you should be smilin
A girl like you should never look so blue
You re everything I see in my dreams
I wouldn t say that to you if it wasn t true

Oh, whoa
I know that you don t know it, but you re fine, so fine
(Fine, so fine)
Oh, whoa
Oh girl, I m gonna show you when you re mine, oh, mine
(Mine, oh, mine)

Treasure, that is what you are
Honey, you re my golden star
You know you can make my wish come true
If you let me treasure you
If you let me treasure you

You are my treasure, you are my treasure
You are my treasure, yeah, you, you, you, you are
You are my treasure, you are my treasure
You are my treasure, yeah, you, you, you, you are

Treasure (you are my treasure), that is what you are (you are my treasure)
(You are my treasure)
Honey, you re my golden star (you, you, you, you are, you are my treasure)
You know you can make my wish come true (you are my treasure)
(You are my treasure)
If you let me treasure you (you, you, you, you are, you are my treasure)
If you let me treasure you (you are my treasure)'),
 (9, 'When I Was Your Man', '00:03:54',5, 
'Same bed, but it feels just a little bit bigger now
Our song on the radio, but it don t sound the same
When our friends talk about you, all it does is just tear me down
Cause my heart breaks a little when I hear your name

It all just sounds like ooh, ooh
Mm, too young, too dumb to realize

That I should ve bought you flowers and held your hand
Shoulda gave you all my hours when I had the chance
Take you to every party, cause all you wanted to do was dance
Now my baby s dancin, but she s dancin with another man

My pride, my ego, my needs, and my selfish ways
Caused a good, strong woman like you to walk out my life
Now I ll never, never get to clean up the mess I made, oh
And that haunts me every time I close my eyes

It all just sounds like ooh, ooh
Mm, too young, too dumb to realize

That I should ve bought you flowers and held your hand
Shoulda gave you all my hours when I had the chance
Take you to every party,  cause all you wanted to do was dance
Now my baby s dancin , but she s dancin  with another man

Although it hurts, I ll be the first to say that I was wrong
Oh, I know I m probably much too late
To try and apologize for my mistakes
But I just want you to know

I hope he buys you flowers, I hope he holds your hand
Give you all his hours when he has the chance
Take you to every party, cause I remember how much you love to dance
Do all the things I should ve done when I was your man

Do all the things I should ve done when I was your man'),
    (10, 'White Christmas', '00:03:03', 1, 
'I m dreaming of a white Christmas
Just like the ones I used to know
Where the treetops glisten and children listen
To hear sleigh bells in the snow

I m dreaming of a white Christmas
With every Christmas card I write
May your days be merry and bright
And may all your Christmases be white

I m dreaming of a white Christmas
Just like the ones I used to know
Where the treetops glisten and children listen
To hear sleigh bells in the snow

I m dreaming of a white Christmas
With every Christmas card I write
May your days be merry and bright (be merry and bright)
And may all your Christmases be white');

insert into cree VALUES 
    (1,1),
    (2,4),
    (3,2),
    (4,1),
    (5,3),
    (6,2),
    (7,4),
    (8,2),
    (9,2),
    (10,3);


insert into utilisateur(pseudo,mail,motdepasse,dateArriver) VALUES 
    ('tamaki', 'tamaki@gmail.com', 'motDePasse2','2005-10-20'),
    ('carotte', 'carotte@gmail.com', 'jeSaisPas07','2007-12-05'),
    ('Amcaisss', 'amcaisss@hotmail.com', '1223334444', '2005-02-08'),
    ('someone', 'NULL@null.com', '78999', '2006-12-18'),
    ('courgette', 'courgette@Outlook.com', 'ilnyapasdemdp', '2024-12-10'),
    ('faim', 'faim@gfaim.fr', 'nourriture', '2000-01-01');

INSERT INTO abonnement VALUES 
    (1,2),
    (2,1),
    (1,4),
    (2,3);

INSERT INTO playlist (titre, description, prive,idU) VALUES
    ('detente', 'playlist relaxante', false, 1),
    ('soiree', 'Pour être de bonne humeur', true, 1),
    ('titre_aimé', 'juste a écouté', true, 3),
    ('musique', 'une envie de repos', false, 4),
    ('a écouter', 'musique a écouter plus tard', true, 3),
    ('Lofi', 'pour se reposer', false, 3),
    ('sport', 'still focus', false, 4),
    ('fun', 'musique fun', false, 1);

INSERT INTO suitGroupe VALUES 
    (1,2),
    (2,1),
    (1,4),
    (3,3);

insert into contient VALUES 
    (1,1),
    (3,2),
    (4,2),
    (4,1),
    (2,1),
    (7,1),
    (6,2),
    (5,1),
    (10,2);

insert into suit VALUES 
    (1,1),
    (1,3),
    (2,4),
    (1,2);

INSERT INTO ecoute(idU, idA, idM) VALUES
    (1,4,7),
    (1,1,5),
    (2,3,1),
    (1,3,4),
    (2,3,1),
    (1,3,4);

INSERT INTO ecoutePlay(idU,idP,idM) VALUES
    (1,2,10),
    (1,2,4);

    