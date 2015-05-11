use GameDB
go

SET NOCOUNT ON
GO

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

IF EXISTS ( select * from sys.tables where name='PlayerTable' )
	DROP TABLE [dbo].[PlayerTable]
GO


CREATE TABLE [dbo].[PlayerTable](
	[playerUID] [int] NOT NULL PRIMARY KEY IDENTITY(100, 1),
	[playerName] [nvarchar](32) NOT NULL DEFAULT (N'noname'),
	[currentPosX] [float] NOT NULL DEFAULT ((0)),
	[currentPosY] [float] NOT NULL DEFAULT ((0)),
	[currentPosZ] [float] NOT NULL DEFAULT ((0)),
	[createTime] [datetime] NOT NULL,
	[isValid] [tinyint] NOT NULL,
	[comment] [nvarchar](256) NULL
)

GO

IF EXISTS ( select * from sys.procedures where name='spCreatePlayer' )
	DROP PROCEDURE [dbo].[spCreatePlayer]
GO

CREATE PROCEDURE [dbo].[spCreatePlayer]
	@name	NVARCHAR(32)
AS
BEGIN
	SET NOCOUNT ON
	INSERT INTO PlayerTable(playerName, createTime, isValid) VALUES (@name, GETDATE(), 1)
	SELECT @@IDENTITY
END
GO

IF EXISTS ( select * from sys.procedures where name='spDeletePlayer' )
	DROP PROCEDURE [dbo].[spDeletePlayer]
GO

CREATE PROCEDURE [dbo].[spDeletePlayer]
	@playerUID	INT
AS
BEGIN
	SET NOCOUNT ON
	DELETE FROM PlayerTable WHERE playerUID=@playerUID
	SELECT @@ROWCOUNT
END
GO

IF EXISTS ( select * from sys.procedures where name='spUpdatePlayerPosition' )
	DROP PROCEDURE [dbo].[spUpdatePlayerPosition]
GO

CREATE PROCEDURE [dbo].[spUpdatePlayerPosition]
	@playerUID	INT,
	@posX	FLOAT,
	@posY	FLOAT,
	@posZ	FLOAT
AS
BEGIN
	SET NOCOUNT ON
	UPDATE PlayerTable SET currentPosX=@posX, currentPosY=@posY, currentPosZ=@posZ WHERE playerUID=@playerUID
	SELECT @@ROWCOUNT
END
GO

IF EXISTS ( select * from sys.procedures where name='spUpdatePlayerComment' )
	DROP PROCEDURE [dbo].[spUpdatePlayerComment]
GO

CREATE PROCEDURE [dbo].[spUpdatePlayerComment]
	@playerUID	INT,
	@comment	NVARCHAR(256)
AS
BEGIN
	SET NOCOUNT ON
	UPDATE PlayerTable SET comment=@comment WHERE playerUID=@playerUID
	SELECT @@ROWCOUNT
END
GO

IF EXISTS ( select * from sys.procedures where name='spUpdatePlayerValid' )
	DROP PROCEDURE [dbo].[spUpdatePlayerValid]
GO

CREATE PROCEDURE [dbo].[spUpdatePlayerValid]
	@playerUID	INT,
	@valid		TINYINT
AS
BEGIN
	SET NOCOUNT ON
	UPDATE PlayerTable SET isValid=@valid WHERE playerUID=@playerUID
	SELECT @@ROWCOUNT
END
GO


IF EXISTS ( select * from sys.procedures where name='spLoadPlayer' )
	DROP PROCEDURE [dbo].[spLoadPlayer]
GO

CREATE PROCEDURE [dbo].[spLoadPlayer]
	@playerUID	INT
AS
BEGIN
	SET NOCOUNT ON
	SELECT [playerName], [currentPosX], [currentPosY], [currentPosZ], [isValid], [comment] FROM PlayerTable WHERE playerUID=@playerUID
END		   
GO		   

--EXEC spCreatePlayer '테스트플레이어'
--GO

--EXEC spUpdatePlayerComment 100, "가나다라 플레이어 코멘트 테스트 kekeke"
--GO

--EXEC spUpdatePlayerValid 100, 1
--GO

--EXEC spLoadPlayer 100
--GO

--EXEC spDeletePlayer 100
--GO

	
