@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Submit a bug</div>


                    <div class="panel-body">
                        <form role="form" method="POST" action="{{ action('BugController@store_bug') }}">
                            {{ csrf_field() }}

                            @if ($errors->any())
                                <div class="alert alert-danger">
                                    <ul>
                                        @foreach ($errors->all() as $error)
                                            <li>{{ $error }}</li>
                                        @endforeach
                                    </ul>
                                </div>
                            @endif

                            <div class="panel panel-default">
                                <div class="panel-body">
                                    <div class="form-group">
                                        <label for="popis_bugu">Description of a bug</label>
                                        <textarea name="popis_bugu" class="form-control" rows="3" id="popis_bugu"
                                                  placeholder="Preferably what causes the bug, what is the root issue">{{ (old('popis_bugu')) ? old('popis_bugu') : '' }}</textarea>

                                    </div>

                                    <div class="form-group">
                                        <label>Severity of a Bug</label>
                                        @foreach($severity as $item)
                                            <div class="radio">
                                                <label>
                                                    <input type="radio" name="zavaznost" id="{{$item}}"
                                                           value="{{$item}}"
                                                           @if (old('zavaznost') == $item) checked @endif>
                                                    {{$item}}
                                                </label>
                                            </div>
                                        @endforeach
                                    </div>

                                    <div class="form-group">
                                        <label>Bug is assigned to module : </label>
                                        <select class="form-control" name="meno_modulu">
                                            @foreach($modules as $module)
                                                <option @if (old('meno_modulu') == $module->meno) selected @endif>
                                                    {{$module->meno}}</option>
                                            @endforeach
                                        </select>
                                    </div>

                                    <div class="form-group">
                                        <label>Type of bug</label>
                                        <select class="form-control" name="typ">
                                            @foreach($bug_types as $type)
                                                <option @if (old('typ') == $type) selected @endif>{{$type}}</option>
                                            @endforeach
                                        </select>
                                    </div>

                                    <div class="form-group">
                                        <label>Rate of vulnerability</label> (Fill out only if bug is a
                                        vulnerability too.)
                                        @foreach($vulner as $item)
                                            <div class="radio">
                                                <label>
                                                    <input type="radio" name="miera_nebezpecenstva" id="{{$item}}"
                                                           value="{{$item}}"
                                                           @if (old('miera_nebezpecenstva') == $item) checked @endif
                                                    >
                                                    {{$item}}
                                                </label>
                                            </div>
                                        @endforeach
                                    </div>
                                </div>
                            </div>


                            <div id="buttons">
                                <button id="submit" type="submit" class="btn btn-default" onclick="convert()">Submit
                                    bug
                                </button>
                                <button type="reset" class="btn btn-default">Reset fields</button>
                            </div>
                        </form>
                    </div>
                </div>
            </div>
        </div>
    </div>

@endsection
