Public Class Puzzle

    Private Sub Puzzle_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim i As Integer = 1
        Do Until i = 82
            Dim t As New TextBox
            With t
                .BorderStyle = BorderStyle.None
                .Multiline = True
                .Show()
                .Tag = i
                .Width = 50
                .Height = 50
                .MaxLength = 1
                .TextAlign = HorizontalAlignment.Center
                .Font = New Font(System.Drawing.FontFamily.GenericSansSerif, 43, FontStyle.Regular, GraphicsUnit.Pixel)
                Dim a As Integer = SudoGen.GetAcrossFromNumber(i)
                If a < 4 Then
                    .Location = New Point(16 + ((a - 1) * 50) + (a - 1), .Location.Y)
                ElseIf a < 7 Then
                    .Location = New Point(16 + 3 + ((a - 1) * 50) + (a - 2), .Location.Y)
                ElseIf a < 10 Then
                    .Location = New Point(16 + 6 + ((a - 1) * 50) + (a - 3), .Location.Y)
                End If
                Dim d As Integer = SudoGen.GetDownFromNumber(i)
                If d < 4 Then
                    .Location = New Point(.Location.X, 16 + ((d - 1) * 50) + (d - 1))
                ElseIf d < 7 Then
                    .Location = New Point(.Location.X, 16 + 3 + ((d - 1) * 50) + (d - 2))
                ElseIf d < 10 Then
                    .Location = New Point(.Location.X, 16 + 6 + ((d - 1) * 50) + (d - 3))
                End If
                Me.Controls.Add(t)
                AddHandler t.KeyPress, AddressOf Boxes_KeyPress
            End With
            i += 1
        Loop
    End Sub

    Private Sub Puzzle_Paint(ByVal sender As Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles Me.Paint
        e.Graphics.FillRectangle(Brushes.Black, 12, 12, 470, 470)
    End Sub

    Public Sub Boxes_KeyPress(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyPressEventArgs)
        If Char.IsDigit(e.KeyChar) = False Or e.KeyChar = CChar("0") Then
            If e.KeyChar = ChrW(Keys.Back) Then
                e.Handled = False
            Else
                e.Handled = True
            End If
        End If
    End Sub

    Private Sub ButtonGenerate_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonGenerate.Click
        Application.DoEvents()
        SudoGen.GenerateGrid()
        Dim l As List(Of Square) = SudoGen.Sudoku
        For Each s As Square In l
            For Each m As Control In Me.Controls
                If m.Tag = s.Index + 1 Then
                    m.Text = s.Value
                End If
            Next
        Next
        'ClearOut(44)
        Application.DoEvents()
    End Sub

    Private Sub ClearOut(ByVal i As Integer)
        Dim CullList As New List(Of Integer)
        CullList = FillList(1)
        Dim CullList2 As New List(Of Integer)
        CullList2 = CullList
        Dim ran As New Random
        Dim l As Integer
        Dim LeftCull As Integer = Math.Round((5 / 9) * i) - 2
        Do Until CullList.Count = (i - LeftCull)
            l = ran.Next(0, CullList.Count)
            CullList.RemoveAt(l)
        Loop
        For Each s As Integer In CullList2
            For Each c As Control In Me.Controls
                If TypeOf (c) Is TextBox Then
                    If c.Tag = s Then
                        c.Text = Nothing
                    End If
                End If
            Next
        Next
        For Each c As Control In Me.Controls
            If TypeOf (c) Is TextBox Then
                If c.Text = Nothing Then
                    Dim a As Integer
                    Dim r As Integer
                    a = SudoGen.GetAcrossFromNumber(CInt(c.Tag))
                    r = (10 - a) - a
                    r = r + (9 * (10 - (2 * SudoGen.GetDownFromNumber(CInt(c.Tag)))))
                    CullList.Add(CInt(c.Tag) + r)
                End If
            End If
        Next
        For Each s As Integer In CullList
            For Each c As Control In Me.Controls
                If TypeOf (c) Is TextBox Then
                    If c.Tag = s Then
                        c.Text = Nothing
                    End If
                End If
            Next
        Next
        For Each c As Control In Me.Controls
            If TypeOf (c) Is TextBox Then
                If c.Text <> Nothing Then
                    c.Enabled = False
                    c.BackColor = Color.LemonChiffon
                Else
                    c.Enabled = True
                    c.BackColor = Color.White
                End If
            End If
        Next
        Dim count As Integer = 0
        For Each c As Control In Me.Controls
            If TypeOf (c) Is TextBox Then
                If c.Text = Nothing Then
                    count += 1
                End If
            End If
        Next
        Me.Text = count
    End Sub

    Private Function FillList(ByVal k As Integer, Optional ByVal Do5 As Boolean = True) As List(Of Integer)
        FillList = New List(Of Integer)
        Dim i As Integer = 0
        Dim j As Integer
        Do Until i = 9
            j = 9 * i
            FillList.Add(k + j)
            FillList.Add(k + j + 1)
            FillList.Add(k + j + 2)
            FillList.Add(k + j + 3)
            If Do5 = True Then
                FillList.Add(k + j + 4)
            End If
            i += 1
        Loop

        Return FillList
    End Function
End Class
