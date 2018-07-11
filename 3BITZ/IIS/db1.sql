INSERT INTO languages (`name`) VALUES ('C');
INSERT INTO languages (`name`) VALUES ('C++');
INSERT INTO languages (`name`) VALUES ('D');
INSERT INTO languages (`name`) VALUES ('Python');
INSERT INTO languages (`name`) VALUES ('Java');
INSERT INTO languages (`name`) VALUES ('PHP');
INSERT INTO languages (`name`) VALUES ('C#');

INSERT INTO `users` (`name`, `email`, `password`, `remember_token`, cele_meno, vek, `group`) VALUES ('user', 'user@bug.dev', '$2y$10$5Rz1UKjXs.YJN4qGjKT.PuT.u9OEAeniaKEGuXfBv1Ul3AkA5t6We', 'A4fmgriXtHuhqSDY8WlcOrGdeePn8ir2OQ3McMuh1KlANhV5a3AfxMJcvKzi', 'Peter Jablko', '22', 'User');
INSERT INTO `users` (`name`, `email`, `password`, `remember_token`, cele_meno, vek, `group`) VALUES ('programmer', 'programmer@bug.dev', '$2y$10$LatmkqG1BUIY3wbULuo70.gg.zZG1TFOWY3xraRwj3DpxNLFe/HVy', 'qZgfxJmykAvJPZpyvQZljXqCydu0vWwz9Xjpy0LSmXueIKcnSp7Rtfo8rNlY', 'Jan Hruska', '27', 'Programmer');
INSERT INTO `users` (`name`, `email`, `password`, `remember_token`, cele_meno, vek, `group`) VALUES ('admin', 'admin@bug.dev', '$2y$10$3yTKOMInN/2IgdpDyKcRHeo3OM4ZWXAny/9mcdvgLCODEyIaSqwxa', '0y3jPMplq85BsllT0YKQzco6DtInIHZCj7Ozkp8jyyCbnG2eqkkUSlSWDozM', 'Juraj Ryza', '31', 'Admin');
INSERT INTO `users` (`name`, `email`, `password`, `remember_token`, cele_meno, vek, `group`) VALUES ('programmer2', 'programmer2@bug.dev', '$2y$10$4WgEpCgUzSuu6C1xR6EEF.boDnexebGSywtffn9raC/XIQ0o72s6a', '01EIbSLslf6fXCJ72JRx6nV1gWCa6gy8yBu0Ph6lMggehXLS9efkVZbBIleS', 'Michal Ruzicka', '57', 'Programmer');

INSERT INTO know (id_lang, `user`) VALUES  (1, 'programmer');
INSERT INTO know (id_lang, `user`) VALUES  (2, 'programmer');
INSERT INTO know (id_lang, `user`) VALUES  (3, 'programmer');
INSERT INTO know (id_lang, `user`) VALUES  (4, 'programmer');
INSERT INTO know (id_lang, `user`) VALUES  (5, 'programmer');
INSERT INTO know (id_lang, `user`) VALUES  (6, 'programmer');
INSERT INTO know (id_lang, `user`) VALUES  (7, 'programmer');
INSERT INTO know (id_lang, `user`) VALUES  (1,'programmer2');
INSERT INTO know (id_lang, `user`) VALUES  (2,'programmer2');
INSERT INTO know (id_lang, `user`) VALUES  (3,'programmer2');
INSERT INTO know (id_lang, `user`) VALUES  (4,'programmer2');
INSERT INTO know (id_lang, `user`) VALUES  (5,'programmer2');
INSERT INTO know (id_lang, `user`) VALUES  (6,'programmer2');
INSERT INTO know (id_lang, `user`) VALUES  (7,'programmer2');
INSERT INTO know (id_lang, `user`) VALUES  (7, 'admin');
INSERT INTO know (id_lang, `user`) VALUES  (3, 'user');
INSERT INTO know (id_lang, `user`) VALUES  (4, 'user');



INSERT INTO modules (chybovost,name_zodpovedny, id_lan, meno) VALUES('1.25', 'programmer',  1,    'i40e');
INSERT INTO modules (chybovost,name_zodpovedny, id_lan, meno) VALUES('2.83', 'programmer',  2,    'libpcap');
INSERT INTO modules (chybovost,name_zodpovedny, id_lan, meno) VALUES('1.14', 'programmer2', 2,    'iostream');
INSERT INTO modules (chybovost,name_zodpovedny, id_lan, meno) VALUES('3.84', 'programmer2', 5,    'gpio');
INSERT INTO modules (chybovost,name_zodpovedny, id_lan, meno) VALUES('0.12', 'programmer2', 2,    'pci');
INSERT INTO modules (chybovost,name_zodpovedny, id_lan, meno) VALUES('0.00', 'programmer',  4,    'virtio');

INSERT INTO `bugs` (`id_bug`, `typ`, `zavaznost`, `popis_bugu`, `id_modul`, `id_patch`) VALUES (450101, 'Logic', 'High', 'Kernel Panic', 1, 5050);
INSERT INTO `bugs` (`id_bug`, `typ`, `zavaznost`, `popis_bugu`, `id_modul`, `id_patch`) VALUES (450102, 'Syntax', 'Medium', 'Module is malfunctioning', 2, 5055);
INSERT INTO `bugs` (`id_bug`, `typ`, `zavaznost`, `popis_bugu`, `id_modul`, `id_patch`) VALUES (450103, 'Not a bug', 'Low', 'Program crashes after submit', 3, 5054);
INSERT INTO `bugs` (`id_bug`, `typ`, `zavaznost`, `popis_bugu`, `id_modul`, `id_patch`) VALUES (450104, 'Interfacing', 'Medium', 'Network does not work', 4, 5056);
INSERT INTO `bugs` (`id_bug`, `typ`, `zavaznost`, `popis_bugu`, `id_modul`, `id_patch`) VALUES (450105, 'Resource', 'Low', 'Blue screen of death', 5, 5052);
INSERT INTO `bugs` (`id_bug`, `typ`, `zavaznost`, `popis_bugu`, `id_modul`, `id_patch`) VALUES (450106, 'Logic', 'High', 'Deadlock of processes', 1, 5056);
INSERT INTO `bugs` (`id_bug`, `typ`, `zavaznost`, `popis_bugu`, `id_modul`, `id_patch`) VALUES (450107, 'Syntax', 'Medium', 'Unexpected behaviour when clicked on button', 2, 5051);
INSERT INTO `bugs` (`id_bug`, `typ`, `zavaznost`, `popis_bugu`, `id_modul`, `id_patch`) VALUES (450108, 'Logic', 'High', 'User is not saved after being created in app', 3, 5053);
INSERT INTO `bugs` (`id_bug`, `typ`, `zavaznost`, `popis_bugu`, `id_modul`, `id_patch`) VALUES (450109, 'Resource', 'High', 'Fails to boot at all', 1, NULL);
INSERT INTO `bugs` (`id_bug`, `typ`, `zavaznost`, `popis_bugu`, `id_modul`, `id_patch`) VALUES (450110, 'Interfacing', 'Medium', 'However, in contrast, Linux is completely an open source project. You can have a look at the source code of a Linux OS, which', 3, NULL);
INSERT INTO `bugs` (`id_bug`, `typ`, `zavaznost`, `popis_bugu`, `id_modul`, `id_patch`) VALUES (450111, 'Syntax', 'Low', 'Let’s face it; Windows OS is vulnerable to different types of attacks (or hacks). However, Linux is not as vulnerable as Windows. It sure isn’t invulnerable, but it is a lot more secure. Although, there’s no rocket science in i', 5, NULL);
INSERT INTO `bugs` (`id_bug`, `typ`, `zavaznost`, `popis_bugu`, `id_modul`, `id_patch`) VALUES (450112, 'Arithmetic', 'Low', 'e way Linux works that makes it a secure operating system. Overall, the process of package management, the concept of repositories, and a couple more features makes it p', 6, NULL);
INSERT INTO `bugs` (`id_bug`, `typ`, `zavaznost`, `popis_bugu`, `id_modul`, `id_patch`) VALUES (450113, 'Logic', 'Medium', 'refore be located at\r\n\r\nthe beginning of this\r\n\r\ndocument!\r\n\r\n\r\nThis part of the\r\n\r\ndocument has stayed the\r\n\r\nsame from version to\r\n\r\nversion.  It shouldn\'t\r\n\r\nbe shown if it doesn\'t\r\n\r\nchange.', 1, NULL);

INSERT INTO `tickets` (`id_ticket`, `stav`, `popis`, `name_prideleny`, `name_vytvoreny`) VALUES (2001, 'Open', 'Crash of system when working in GNOME', 'programmer', 'user');
INSERT INTO `tickets` (`id_ticket`, `stav`, `popis`, `name_prideleny`, `name_vytvoreny`) VALUES (2002, 'Assigned', 'Terminal is twitching after restart', 'programmer2', 'user');
INSERT INTO `tickets` (`id_ticket`, `stav`, `popis`, `name_prideleny`, `name_vytvoreny`) VALUES (2003, 'New', 'BSOD on linux', NULL, 'user');
INSERT INTO `tickets` (`id_ticket`, `stav`, `popis`, `name_prideleny`, `name_vytvoreny`) VALUES (2004, 'Closed', 'Virtualbox fails to boot SunOS', 'programmer', 'user');
INSERT INTO `tickets` (`id_ticket`, `stav`, `popis`, `name_prideleny`, `name_vytvoreny`) VALUES (2005, 'Need Info', 'Docker does not support version 2.2.54.156441.2', 'programmer2', 'programmer');
INSERT INTO `tickets` (`id_ticket`, `stav`, `popis`, `name_prideleny`, `name_vytvoreny`) VALUES (2007, 'New', '1. Open Source Nature What is it like when you buy a car, but you cannot see what’s under the hood? Similar is the case with when you use a Windows-powered system.', NULL, 'user');
INSERT INTO `tickets` (`id_ticket`, `stav`, `popis`, `name_prideleny`, `name_vytvoreny`) VALUES (2008, 'New', 'When you have Windows installed, you need to download/purchase an Antivirus program to keep your computer safe from hackers and malware. However, Linux does not r', NULL, 'user');
INSERT INTO `tickets` (`id_ticket`, `stav`, `popis`, `name_prideleny`, `name_vytvoreny`) VALUES (2009, 'New', '3. Can revive older computers\r\n\r\nAs the operating systems evolve, so do their hardware requirements increase exponentially.', NULL, 'user');


INSERT INTO `descriptions` (`id_ticket`, `id_bug`) VALUES (2002, 450103);
INSERT INTO `descriptions` (`id_ticket`, `id_bug`) VALUES (2003, 450104);
INSERT INTO `descriptions` (`id_ticket`, `id_bug`) VALUES (2001, 450105);
INSERT INTO `descriptions` (`id_ticket`, `id_bug`) VALUES (2002, 450106);
INSERT INTO `descriptions` (`id_ticket`, `id_bug`) VALUES (2002, 450107);
INSERT INTO `descriptions` (`id_ticket`, `id_bug`) VALUES (2001, 450108);
INSERT INTO `descriptions` (`id_ticket`, `id_bug`) VALUES (2003, 450109);
INSERT INTO `descriptions` (`id_ticket`, `id_bug`) VALUES (2007, 450110);
INSERT INTO `descriptions` (`id_ticket`, `id_bug`) VALUES (2009, 450110);
INSERT INTO `descriptions` (`id_ticket`, `id_bug`) VALUES (2007, 450111);
INSERT INTO `descriptions` (`id_ticket`, `id_bug`) VALUES (2007, 450112);
INSERT INTO `descriptions` (`id_ticket`, `id_bug`) VALUES (2008, 450113);


INSERT INTO vulnerabilities (id_vulnerability, miera_nebezpecenstva) VALUES('450101', 'High');

INSERT INTO `patches` (`id_patch`, `schvalenie`, `datum_vydania`, `datum_zavedenia`, `name_vydany`, `name_schvaleny`, `diff`) VALUES (5050, 0, '2017-03-12', NULL, 'user', NULL, NULL);
INSERT INTO `patches` (`id_patch`, `schvalenie`, `datum_vydania`, `datum_zavedenia`, `name_vydany`, `name_schvaleny`, `diff`) VALUES (5051, 1, '2017-07-11', '2017-08-12', 'user', 'programmer2', '+ int a = 5;');
INSERT INTO `patches` (`id_patch`, `schvalenie`, `datum_vydania`, `datum_zavedenia`, `name_vydany`, `name_schvaleny`, `diff`) VALUES (5052, 1, '2017-06-10', '2017-06-12', 'programmer', 'programmer2', '- void kill();');
INSERT INTO `patches` (`id_patch`, `schvalenie`, `datum_vydania`, `datum_zavedenia`, `name_vydany`, `name_schvaleny`, `diff`) VALUES (5053, 1, '2017-05-09', '2017-07-12', 'programmer2', 'programmer', '+int a,b,c;');
INSERT INTO `patches` (`id_patch`, `schvalenie`, `datum_vydania`, `datum_zavedenia`, `name_vydany`, `name_schvaleny`, `diff`) VALUES (5054, 1, '2017-04-08', '2017-08-12', 'programmer2', 'programmer', '+/*dsfasfdsafdsafdsaf*/');
INSERT INTO `patches` (`id_patch`, `schvalenie`, `datum_vydania`, `datum_zavedenia`, `name_vydany`, `name_schvaleny`, `diff`) VALUES (5055, 1, '2017-03-15', NULL, 'programmer', NULL, NULL);
INSERT INTO `patches` (`id_patch`, `schvalenie`, `datum_vydania`, `datum_zavedenia`, `name_vydany`, `name_schvaleny`, `diff`) VALUES (5056, 0, '2017-12-04', NULL, 'user', NULL, '0a1,6\r\n> This is an important\r\n> notice! It should\r\n> therefore be located at\r\n> the beginning of this\r\n> document!\r\n>\r\n8,14c14\r\n< compress the size of the\r\n< changes.\r\n<\r\n< This paragraph contains\r\n< text that is outdated.\r\n< It will be deleted in the\r\n< near future.\r\n---\r\n> compress anything.\r\n17c17\r\n< check this dokument. On\r\n---\r\n> check this document. On\r\n24a25,28\r\n>');


INSERT INTO `tests` (`id`, `datum_zaciatku`, `datum_konca`, `hodnotenie`, `id_patch`, `napisal`, `name_schvaleny`, `code`) VALUES (1, '2014-03-01', '2017-03-01', 98.60, 5050, 'user', 'programmer', '[TestFixture]\r\npublic class CalculatorTests\r\n{\r\n    [Test]\r\n    public void TestPressEquals()\r\n    {\r\n        Calculator calculator = new Calculator();\r\n        calculator.Enter(2m);\r\n        calculator.PressPlus();\r\n        calculator.Enter(2m);\r\n        calculator.PressEquals();\r\n        Assert.AreEqual(4m, calculator.Display);\r\n    }\r\n}');
INSERT INTO `tests` (`id`, `datum_zaciatku`, `datum_konca`, `hodnotenie`, `id_patch`, `napisal`, `name_schvaleny`, `code`) VALUES (2, '2017-04-01', '2017-04-02', 91.80, 5050, 'user', NULL, '[Test]\r\n        public void TestPressEquals2()\r\n        {\r\n            var calculator = new Calculator();\r\n            calculator.Enter(2);\r\n            calculator.PressPlus();\r\n            calculator.Enter(2);\r\n            calculator.PressEquals();\r\n            Assert.AreEqual(4, calculator.Display,\r\n                "When adding 2 + 2, expected 4 but found {0}.", calculator.Display);\r\n        }');
INSERT INTO `tests` (`id`, `datum_zaciatku`, `datum_konca`, `hodnotenie`, `id_patch`, `napisal`, `name_schvaleny`, `code`) VALUES (3, '2017-04-01', NULL, 0.00, 5052, 'user', NULL, '[Test]\r\n        public void TestPressEquals2()\r\n        {\r\n            var value1 = 2m;\r\n            var value2 = 2m;\r\n            var calculator = new Calculator();\r\n            calculator.Enter(value1);\r\n            calculator.PressPlus();\r\n            calculator.Enter(value2);\r\n            calculator.PressEquals();\r\n            decimal expected = 4m;\r\n            decimal actual = calculator.Display;\r\n            Assert.AreEqual(expected, actual,\r\n                "When adding {0} + {1}, expected {2} but found {3}.", value1, value2, expected, actual);\r\n        }');
INSERT INTO `tests` (`id`, `datum_zaciatku`, `datum_konca`, `hodnotenie`, `id_patch`, `napisal`, `name_schvaleny`, `code`) VALUES (4, '2017-03-02', '2017-03-03', 100.00, 5051, 'user', NULL, '[Test]\r\n        public void TestPressEquals()\r\n        {\r\n            // Arrange\r\n            decimal value1 = 2m;\r\n            decimal value2 = 2m;\r\n            decimal expected = 4m;\r\n            var calculator = new Calculator();\r\n\r\n            // Act\r\n            calculator.Enter(value1);\r\n            calculator.PressPlus();\r\n            calculator.Enter(value2);\r\n            calculator.PressEquals();\r\n            decimal actual = calculator.Display;\r\n\r\n            // Assert\r\n            Assert.AreEqual(expected, actual,\r\n                            "When adding {0} + {1}, expected {2} but found {3}.", value1, value2, expected, actual);\r\n        }');
INSERT INTO `tests` (`id`, `datum_zaciatku`, `datum_konca`, `hodnotenie`, `id_patch`, `napisal`, `name_schvaleny`, `code`) VALUES (5, '2017-01-07', '2017-05-07', 100.00, 5053, 'user', NULL, '[Test]\r\n        public void PressEquals_AddingTwoPlusTwo_ReturnsFour()\r\n        {\r\n            // Arrange\r\n            decimal value1 = 2m;\r\n            decimal value2 = 2m;\r\n            decimal expected = 4m;\r\n            var calculator = new Calculator();\r\n\r\n            // Act\r\n            calculator.Enter(value1);\r\n            calculator.PressPlus();\r\n            calculator.Enter(value2);\r\n            calculator.PressEquals();\r\n            decimal actual = calculator.Display;\r\n\r\n            // Assert\r\n            Assert.AreEqual(expected, actual,\r\n                            "When adding {0} + {1}, expected {2} but found {3}.", value1, value2, expected, actual);\r\n        }');
INSERT INTO `tests` (`id`, `datum_zaciatku`, `datum_konca`, `hodnotenie`, `id_patch`, `napisal`, `name_schvaleny`, `code`) VALUES (6, '2017-03-13', '2018-03-13', 9.60, 5052, 'user', 'programmer', '[TestFixutre]\r\npublic class WhenAddingValueToTheContainer()\r\n{\r\n\r\n[TestFixtureSetup]\r\nSetupContext()\r\n{\r\n// Create sample data\r\n// Act on it\r\n}\r\n\r\n[Test]\r\npublic void ShouldContainOneElement()\r\n{\r\nAssert.That(_container.Items, Has.Count.EqualTo(1));\r\n}\r\n\r\n[Test]\r\npublic void ShouldHaveTotalValueEqualTo100()\r\n{\r\nAssert.That(_container.TotalValue, Is.EqualTo(100));\r\n}');


INSERT INTO programmers (`name`, produkt, `rank`) VALUES ('programmer', 'OpenStack', 8);
INSERT INTO programmers (`name`, produkt, `rank`) VALUES ('programmer2', 'Libvirt', 4);

INSERT INTO bfu (`name`, odmena, karma) VALUES ('user', 1500, 123);